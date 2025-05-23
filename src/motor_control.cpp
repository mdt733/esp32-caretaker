#include "motor_control.h"
#include <math.h>

// Глобальная переменная для режима управления
ControlMode currentControlMode = ControlMode::JOYSTICK;

// Статические переменные для хранения предыдущих значений сигналов двигателей
static int lastLeftMotorValue = 0;
static int lastRightMotorValue = 0;

// Вспомогательная функция для нелинейного преобразования значений джойстика
float processJoystickAxis(float value) {
    // Применяем мертвую зону
    if (fabs(value) < MOTOR_DEADZONE) {
        return 0.0f;
    }
    
    // Нормализуем значение после мертвой зоны
    float normalized = (fabs(value) - MOTOR_DEADZONE) / (1.0f - MOTOR_DEADZONE);
    
    // Применяем квадратичную характеристику для более точного контроля на малых скоростях
    normalized = normalized * normalized;
    
    // Масштабируем к диапазону PWM с учетом минимальной мощности
    float pwm = MOTOR_MIN_POWER + normalized * (MOTOR_MAX_POWER - MOTOR_MIN_POWER);
    
    // Возвращаем с учетом знака исходного значения
    return value > 0 ? pwm : -pwm;
}

void initMotors() {
    // Setup PWM channels
    ledcSetup(MOTOR_PWM_CHANNEL_LEFT1, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_LEFT2, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_RIGHT1, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_RIGHT2, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    
    // Attach PWM channels to pins
    ledcAttachPin(MOTOR_LEFT_IN1, MOTOR_PWM_CHANNEL_LEFT1);
    ledcAttachPin(MOTOR_LEFT_IN2, MOTOR_PWM_CHANNEL_LEFT2);
    ledcAttachPin(MOTOR_RIGHT_IN1, MOTOR_PWM_CHANNEL_RIGHT1);
    ledcAttachPin(MOTOR_RIGHT_IN2, MOTOR_PWM_CHANNEL_RIGHT2);
    
    // Initially stop motors
    stopMotors();
}

void setMotorSpeed(int pwmLeft, int pwmRight) {
    // Проверяем, изменились ли значения моторов
    bool valuesChanged = (pwmLeft != lastLeftMotorValue) || (pwmRight != lastRightMotorValue);
    
    // Сохраняем новые значения
    lastLeftMotorValue = pwmLeft;
    lastRightMotorValue = pwmRight;
    
    // Left motor
    if (pwmLeft > 0) {
        ledcWrite(MOTOR_PWM_CHANNEL_LEFT1, pwmLeft);
        ledcWrite(MOTOR_PWM_CHANNEL_LEFT2, 0);
    } else {
        ledcWrite(MOTOR_PWM_CHANNEL_LEFT1, 0);
        ledcWrite(MOTOR_PWM_CHANNEL_LEFT2, -pwmLeft);
    }
    
    // Right motor
    if (pwmRight > 0) {
        ledcWrite(MOTOR_PWM_CHANNEL_RIGHT1, pwmRight);
        ledcWrite(MOTOR_PWM_CHANNEL_RIGHT2, 0);
    } else {
        ledcWrite(MOTOR_PWM_CHANNEL_RIGHT1, 0);
        ledcWrite(MOTOR_PWM_CHANNEL_RIGHT2, -pwmRight);
    }
    
    // Логируем изменения, если они произошли
    if (valuesChanged) {
        // Нормализуем значения к диапазону [-1.0, 1.0] для вывода
        float normLeftMotor = pwmLeft / (float)MOTOR_MAX_POWER;
        float normRightMotor = pwmRight / (float)MOTOR_MAX_POWER;
        
        Serial.printf("MOTORS: левый=%.2f, правый=%.2f\n", normLeftMotor, normRightMotor);
    }
}

void processJoystickControl(float x, float y) {
    // Обработка входных значений
    float processedX = processJoystickAxis(x);
    float processedY = processJoystickAxis(y);
    
    float leftMotor = 0;
    float rightMotor = 0;
    
    // Проверяем режим поворота на месте
    if (fabs(x) > MOTOR_TURN_THRESHOLD && fabs(y) < MOTOR_DEADZONE) {
        // Разворот на месте - один мотор вперед, другой назад
        float turnPower = fabs(processedX);
        if (x > 0) {
            // Поворот вправо
            leftMotor = turnPower;
            rightMotor = -turnPower;
        } else {
            // Поворот влево
            leftMotor = -turnPower;
            rightMotor = turnPower;
        }
    } else {
        // Обычное движение
        leftMotor = processedY;
        rightMotor = processedY;
        
        // Применяем поворот путем замедления одной гусеницы
        if (fabs(processedX) > 0) {
            float turnFactor = 1.0f - fabs(processedX / MOTOR_MAX_POWER);
            if (x > 0) {
                // Поворот вправо
                rightMotor *= turnFactor;
            } else {
                // Поворот влево
                leftMotor *= turnFactor;
            }
        }
    }
    
    // Применяем ограничения и отправляем на моторы
    int pwmLeft = constrain(leftMotor, -MOTOR_MAX_POWER, MOTOR_MAX_POWER);
    int pwmRight = constrain(rightMotor, -MOTOR_MAX_POWER, MOTOR_MAX_POWER);
    
    setMotorSpeed(pwmLeft, pwmRight);
}

// Новая функция для получения значений моторов без отправки на драйвер
void processJoystickControlWithValues(float x, float y, int* leftMotor, int* rightMotor) {
    // Обработка входных значений
    float processedX = processJoystickAxis(x);
    float processedY = processJoystickAxis(y);
    
    float leftMotorValue = 0;
    float rightMotorValue = 0;
    
    // Проверяем режим поворота на месте
    if (fabs(x) > MOTOR_TURN_THRESHOLD && fabs(y) < MOTOR_DEADZONE) {
        // Разворот на месте - один мотор вперед, другой назад
        float turnPower = fabs(processedX);
        if (x > 0) {
            // Поворот вправо
            leftMotorValue = turnPower;
            rightMotorValue = -turnPower;
        } else {
            // Поворот влево
            leftMotorValue = -turnPower;
            rightMotorValue = turnPower;
        }
    } else {
        // Обычное движение
        leftMotorValue = processedY;
        rightMotorValue = processedY;
        
        // Применяем поворот путем замедления одной гусеницы
        if (fabs(processedX) > 0) {
            float turnFactor = 1.0f - fabs(processedX / MOTOR_MAX_POWER);
            if (x > 0) {
                // Поворот вправо
                rightMotorValue *= turnFactor;
            } else {
                // Поворот влево
                leftMotorValue *= turnFactor;
            }
        }
    }
    
    // Применяем ограничения и отправляем на драйвер
    *leftMotor = constrain(leftMotorValue, -MOTOR_MAX_POWER, MOTOR_MAX_POWER);
    *rightMotor = constrain(rightMotorValue, -MOTOR_MAX_POWER, MOTOR_MAX_POWER);
    
    // Также отправляем на моторы
    setMotorSpeed(*leftMotor, *rightMotor);
}

void processSlidersControl(float leftSlider, float rightSlider) {
    // Apply deadzone
    if (abs(leftSlider) < MOTOR_DEADZONE) leftSlider = 0;
    if (abs(rightSlider) < MOTOR_DEADZONE) rightSlider = 0;

    // Apply quadratic characteristic for better control at low speeds
    float leftPower = leftSlider * abs(leftSlider);   // Preserves sign but makes control more precise
    float rightPower = rightSlider * abs(rightSlider);

    // Scale to PWM range and apply minimum power threshold
    int leftPWM = 0;
    int rightPWM = 0;

    if (leftPower != 0) {
        leftPWM = (int)(abs(leftPower) * (MOTOR_MAX_POWER - MOTOR_MIN_POWER) + MOTOR_MIN_POWER);
        if (leftPower < 0) leftPWM = -leftPWM;
    }

    if (rightPower != 0) {
        rightPWM = (int)(abs(rightPower) * (MOTOR_MAX_POWER - MOTOR_MIN_POWER) + MOTOR_MIN_POWER);
        if (rightPower < 0) rightPWM = -rightPWM;
    }

    setMotorSpeed(leftPWM, rightPWM);
}

void stopMotors() {
    // Set all PWM channels to 0
    ledcWrite(MOTOR_PWM_CHANNEL_LEFT1, 0);
    ledcWrite(MOTOR_PWM_CHANNEL_LEFT2, 0);
    ledcWrite(MOTOR_PWM_CHANNEL_RIGHT1, 0);
    ledcWrite(MOTOR_PWM_CHANNEL_RIGHT2, 0);
} 