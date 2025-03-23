#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>
    <meta name="mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
    <meta name="format-detection" content="telephone=no">
    <meta name="msapplication-tap-highlight" content="no">
    <meta name="HandheldFriendly" content="true">
    <title>ESP32-CAM caretaker</title>
    <style>
        :root {
            --primary-color: #4CAF50;
            --danger-color: #f44336;
            --control-bg: rgba(0, 0, 0, 0.5);
            --text-color: #ffffff;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            -webkit-tap-highlight-color: transparent;
            touch-action: none !important;
            -ms-touch-action: none !important;
            -webkit-touch-callout: none !important;
            -webkit-user-select: none !important;
            -moz-user-select: none !important;
            -ms-user-select: none !important;
            user-select: none !important;
            -webkit-overflow-scrolling: none !important;
        }

        body { 
            font-family: Arial, sans-serif;
            background-color: #000;
            min-height: 100vh;
            overflow: hidden;
            color: var(--text-color);
            touch-action: none !important;
            -ms-touch-action: none !important;
            overscroll-behavior: none !important;
            position: fixed;
            width: 100%;
            height: 100%;
        }

        .container {
            position: relative;
            width: 100vw;
            height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            touch-action: none;
        }

        .video-container {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            z-index: 1;
            background: #000;
        }

        .stream-placeholder {
            width: 100%;
            height: 100%;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #666;
            font-size: 1.2em;
        }

        #stream {
            width: 100%;
            height: 100%;
            object-fit: contain;
            display: none;
        }

        .controls-overlay {
            position: absolute;
            z-index: 2;
            width: 100%;
            height: 100%;
            pointer-events: none;
        }

        .controls-top {
            position: absolute;
            top: 20px;
            left: 20px;
            display: flex;
            align-items: center;
            gap: 16px;
            pointer-events: all;
            background: var(--control-bg);
            padding: 8px;
            border-radius: 8px;
            backdrop-filter: blur(5px);
        }

        .controls-right {
            position: absolute;
            right: 20px;
            top: 50%;
            transform: translateY(-50%);
            pointer-events: all;
            display: flex;
            flex-direction: column;
            gap: 16px;
        }

        .control-mode-switch {
            position: absolute;
            top: 20px;
            right: 20px;
            z-index: 10;
            pointer-events: all;
            background: var(--control-bg);
            padding: 8px;
            border-radius: 8px;
            backdrop-filter: blur(5px);
            display: flex;
            gap: 10px;
        }

        .control-mode-switch .button {
            min-width: 44px;
            font-size: 20px;
            padding: 8px;
            line-height: 1;
        }

        .joystick-container {
            width: 200px;
            height: 200px;
            background: var(--control-bg);
            border-radius: 8px;
            padding: 20px;
            backdrop-filter: blur(5px);
            display: none;
        }

        .joystick-container.active {
            display: block;
        }

        #joystick {
            width: 100%;
            height: 100%;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 50%;
            position: relative;
            touch-action: none;
        }

        #stick {
            width: 40%;
            height: 40%;
            background: rgba(255, 255, 255, 0.8);
            border-radius: 50%;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            cursor: pointer;
        }

        .sliders-container {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            pointer-events: none;
            display: none;
        }

        .sliders-container.active {
            display: block;
        }

        .slider-vertical {
            position: absolute;
            width: 100px;
            height: calc(100vh - 110px); /* 100px сверху + 50px снизу */
            background: rgba(255, 255, 255, 0.1);
            border-radius: 50px;
            padding: 10px;
            box-sizing: border-box;
            pointer-events: all;
            top: 80px; /* Отступ сверху */
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        .slider-left {
            left: 40px;
        }

        .slider-right {
            right: 40px;
        }

        .slider-thumb {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            width: 80px;
            height: 80px;
            background: rgba(255, 255, 255, 0.8);
            border-radius: 50%;
            cursor: pointer;
        }

        .button {
            padding: 8px 16px;
            border: none;
            border-radius: 6px;
            cursor: pointer;
            font-size: 14px;
            background: rgba(255, 255, 255, 0.1);
            color: var(--text-color);
            transition: all 0.3s;
            white-space: nowrap;
            min-width: 40px;
            text-align: center;
            touch-action: manipulation;
            user-select: none;
            -webkit-user-select: none;
            opacity: 0.7;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .button:active {
            background: rgba(255, 255, 255, 0.3);
            opacity: 1;
        }

        .button.active {
            background: rgba(255, 255, 255, 0.2);
            opacity: 1;
        }

        .button.led-off {
            opacity: 0.4;
            text-decoration: line-through;
        }

        .button.led-mid {
            opacity: 0.7;
        }

        .button.led-high {
            opacity: 1;
            text-shadow: 0 0 8px rgba(255, 255, 255, 0.5);
        }

        .status {
            position: absolute;
            bottom: 20px;
            left: 50%;
            transform: translateX(-50%);
            padding: 10px 20px;
            border-radius: 8px;
            display: none;
            z-index: 100;
            background: var(--control-bg);
            backdrop-filter: blur(5px);
            pointer-events: all;
        }

        .status.error {
            background: rgba(244, 67, 54, 0.8);
        }

        .status.success {
            background: rgba(76, 175, 80, 0.8);
        }

        .bt-status {
            display: none;
            color: #fff;
            font-size: 14px;
            padding: 4px 8px;
            background: rgba(0, 0, 0, 0.5);
            border-radius: 4px;
            margin-left: 10px;
        }
        
        .bt-status.active {
            display: block;
        }
        
        .bt-active {
            background: rgba(76, 175, 80, 0.2);
        }

        /* New styles for settings panel */
        .settings-panel {
            position: fixed;
            top: 0;
            left: -320px;
            width: 320px;
            height: 100%;
            background: rgba(0, 0, 0, 0.85);
            backdrop-filter: blur(10px);
            z-index: 1000;
            transition: left 0.3s ease;
            padding: 20px;
            overflow-y: auto;
            box-shadow: 5px 0 15px rgba(0, 0, 0, 0.5);
        }
        
        .settings-panel.active {
            left: 0;
        }
        
        .settings-close {
            position: absolute;
            top: 15px;
            right: 15px;
            font-size: 24px;
            cursor: pointer;
            color: #fff;
            opacity: 0.7;
        }
        
        .settings-close:hover {
            opacity: 1;
        }
        
        .settings-title {
            font-size: 18px;
            margin-bottom: 20px;
            font-weight: bold;
            border-bottom: 1px solid rgba(255, 255, 255, 0.2);
            padding-bottom: 10px;
        }
        
        .settings-section {
            margin-bottom: 25px;
        }
        
        .settings-option {
            margin-bottom: 20px;
        }
        
        .settings-slider-container {
            width: 100%;
            margin: 10px 0;
        }
        
        .settings-slider {
            width: 100%;
            -webkit-appearance: none;
            height: 6px;
            border-radius: 3px;
            background: rgba(255, 255, 255, 0.2);
            outline: none;
        }
        
        .settings-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: var(--primary-color);
            cursor: pointer;
        }
        
        .settings-slider::-moz-range-thumb {
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: var(--primary-color);
            cursor: pointer;
            border: none;
        }
        
        .settings-slider-value {
            text-align: center;
            margin-top: 5px;
            font-size: 12px;
            opacity: 0.7;
        }
        
        .settings-select {
            background: rgba(255, 255, 255, 0.1);
            border: 1px solid rgba(255, 255, 255, 0.2);
            color: white;
            padding: 6px 10px;
            border-radius: 4px;
            width: 100%;
        }
        
        .settings-select option {
            background-color: #333;
        }
        
        /* Control mode tab selector */
        .control-mode-tabs {
            display: flex;
            width: 100%;
            margin-bottom: 10px;
        }
        
        .control-mode-tab {
            flex: 1;
            text-align: center;
            padding: 8px 0;
            border: 1px solid rgba(255, 255, 255, 0.2);
            cursor: pointer;
            opacity: 0.7;
            background: rgba(255, 255, 255, 0.05);
        }
        
        .control-mode-tab:first-child {
            border-radius: 4px 0 0 4px;
        }
        
        .control-mode-tab:last-child {
            border-radius: 0 4px 4px 0;
        }
        
        .control-mode-tab.active {
            background: rgba(255, 255, 255, 0.2);
            opacity: 1;
        }
        
        /* Icon styles */
        .icon-button {
            font-size: 20px;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 10px;
            min-width: 44px;
            min-height: 44px;
        }
        
        .icon {
            width: 24px;
            height: 24px;
            display: block;
        }
        
        .material-icons {
            font-family: 'Material Icons';
            font-weight: normal;
            font-style: normal;
            font-size: 24px;
            line-height: 1;
            letter-spacing: normal;
            text-transform: none;
            display: inline-block;
            white-space: nowrap;
            word-wrap: normal;
            direction: ltr;
            -webkit-font-smoothing: antialiased;
        }
        
        .overlay {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background: rgba(0, 0, 0, 0.5);
            z-index: 999;
            display: none;
        }
        
        .overlay.active {
            display: block;
        }
    </style>
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
</head>
<body>
    <div class="container">
        <div class="video-container">
            <div class="stream-placeholder" id="stream-placeholder">
                No video stream
            </div>
            <img id="stream">
        </div>
        
        <div class="controls-overlay">
            <div class="controls-top">
                <button class="button icon-button" id="stream-toggle" title="Start/Stop Stream">
                    <span class="material-icons">play_arrow</span>
                </button>
                <button class="button icon-button" id="settings-toggle" title="Settings">
                    <span class="material-icons">settings</span>
                </button>
            </div>

            <div class="control-mode-switch">
                <button class="button icon-button" id="fullscreen-button" title="Fullscreen">
                    <span class="material-icons">fullscreen</span>
                </button>
            </div>
            
            <div class="controls-right">
                <div class="joystick-container active" id="joystick-control">
                    <div id="joystick">
                        <div id="stick"></div>
                    </div>
                </div>
            </div>

            <div class="sliders-container" id="sliders-control">
                <div class="slider-vertical slider-left">
                    <div class="slider-thumb" id="left-thumb"></div>
                </div>
                <div class="slider-vertical slider-right">
                    <div class="slider-thumb" id="right-thumb"></div>
                </div>
            </div>

            <div id="status" class="status"></div>
        </div>
        
        <!-- Settings Panel -->
        <div class="overlay" id="settings-overlay"></div>
        <div class="settings-panel" id="settings-panel">
            <div class="settings-close" id="settings-close">&times;</div>
            <div class="settings-title">Settings</div>
            
            <div class="settings-section">
                <select id="resolution-select" class="settings-select">
                    <option value="QVGA">QVGA (320x240)</option>
                    <option value="VGA">VGA (640x480)</option>
                    <option value="SVGA">SVGA (800x600)</option>
                    <option value="XGA">XGA (1024x768)</option>
                    <option value="SXGA">SXGA (1280x1024)</option>
                </select>
            </div>
            
            <div class="settings-section">
                <div class="settings-slider-container">
                    <input type="range" min="10" max="64" value="12" class="settings-slider" id="quality-slider">
                    <div class="settings-slider-value" id="quality-value">12 (Higher quality)</div>
                </div>
            </div>
            
            <div class="settings-section">
                <div class="control-mode-tabs">
                    <div class="control-mode-tab active" data-mode="joystick" id="joystick-tab">Joystick</div>
                    <div class="control-mode-tab" data-mode="sliders" id="sliders-tab">Sliders</div>
                </div>
            </div>
            
            <div class="settings-section">
                <button class="button" id="bt-toggle">Bluetooth</button>
                <div class="bt-status">Bluetooth: Выключен</div>
            </div>
            
            <div class="settings-section">
                <div class="settings-slider-container">
                    <input type="range" min="0" max="100" value="0" class="settings-slider" id="led-slider">
                    <div class="settings-slider-value" id="led-value">LED: Off</div>
                </div>
            </div>
        </div>
    </div>

    <script>
        const streamImg = document.getElementById('stream');
        const statusDiv = document.getElementById('status');
        const streamPlaceholder = document.getElementById('stream-placeholder');
        const streamToggle = document.getElementById('stream-toggle');
        const controlMode = document.getElementById('control-mode');
        const joystickControl = document.getElementById('joystick-control');
        const slidersControl = document.getElementById('sliders-control');
        const leftThumb = document.getElementById('left-thumb');
        const rightThumb = document.getElementById('right-thumb');
        const joystick = document.getElementById('joystick');
        const stick = document.getElementById('stick');
        const fullscreenButton = document.getElementById('fullscreen-button');
        const btStatus = document.querySelector('.bt-status');
        const ledButton = document.getElementById('led-button');
        const btToggle = document.getElementById('bt-toggle');

        // Settings panel elements
        const settingsToggle = document.getElementById('settings-toggle');
        const settingsPanel = document.getElementById('settings-panel');
        const settingsClose = document.getElementById('settings-close');
        const settingsOverlay = document.getElementById('settings-overlay');
        const resolutionSelect = document.getElementById('resolution-select');
        const qualitySlider = document.getElementById('quality-slider');
        const qualityValue = document.getElementById('quality-value');
        const ledSlider = document.getElementById('led-slider');
        const ledValue = document.getElementById('led-value');
        const joystickTab = document.getElementById('joystick-tab');
        const slidersTab = document.getElementById('sliders-tab');

        let isStoppingStream = false;
        let isStreaming = false;
        let isJoystickMode = true;
        let isDragging = false;
        let currentX = 0;
        let currentY = 0;
        let lastJoystickSendTime = 0;
        let pendingJoystickSend = null;
        let activeThumb = null;
        let startY = 0;
        let startTop = 0;
        let lastSendTime = 0;
        let pendingSend = null;
        let activeTouches = new Map(); // Хранит активные тачи для каждого слайдера
        let ledBrightness = 0; // 0-100%

        const THROTTLE_MS = 400; // Common throttle time for both joystick and sliders

        let btPollingInterval = 2000; // 2 секунды по умолчанию
        let btConnected = false; // Флаг состояния подключения
        let isBtActive = false;  // Флаг активности Bluetooth

        // Settings panel toggle
        function toggleSettingsPanel() {
            settingsPanel.classList.toggle('active');
            settingsOverlay.classList.toggle('active');
        }

        settingsToggle.addEventListener('click', toggleSettingsPanel);
        settingsClose.addEventListener('click', toggleSettingsPanel);
        settingsOverlay.addEventListener('click', toggleSettingsPanel);

        // Quality slider handling
        qualitySlider.addEventListener('input', function() {
            const value = this.value;
            qualityValue.textContent = value + (value < 30 ? " (Higher quality)" : " (Lower quality)");
        });

        qualitySlider.addEventListener('change', function() {
            const quality = this.value;
            sendQualitySettings();
        });

        resolutionSelect.addEventListener('change', function() {
            sendQualitySettings();
        });

        function sendQualitySettings() {
            const resolution = resolutionSelect.value;
            const quality = qualitySlider.value;
            
            fetch(`/quality?resolution=${resolution}&quality=${quality}`)
                .then(response => response.text())
                .then(result => {
                    showStatus(`Quality updated: ${resolution}, compression: ${quality}`);
                })
                .catch(error => {
                    showStatus('Failed to change quality: ' + error, true);
                });
        }

        function showStatus(message, isError = false) {
            statusDiv.textContent = message;
            statusDiv.style.display = 'block';
            statusDiv.className = 'status ' + (isError ? 'error' : 'success');
            setTimeout(() => {
                statusDiv.style.display = 'none';
            }, 3000);
        }

        function toggleStream() {
            if (!isStreaming) {
                startStream();
            } else {
                stopStream();
            }
        }

        function startStream() {
            isStoppingStream = false;
            isStreaming = true;
            streamImg.src = '/stream';
            streamImg.style.display = 'block';
            streamPlaceholder.style.display = 'none';
            streamToggle.innerHTML = '<span class="material-icons">stop</span>';
            streamToggle.classList.add('active');
            showStatus('Starting stream...');

            streamImg.onerror = function() {
                if (!isStoppingStream) {
                    showStatus('Failed to start stream', true);
                    streamImg.style.display = 'none';
                    streamPlaceholder.style.display = 'flex';
                    isStreaming = false;
                    streamToggle.innerHTML = '<span class="material-icons">play_arrow</span>';
                    streamToggle.classList.remove('active');
                }
            };
        }

        function stopStream() {
            isStoppingStream = true;
            isStreaming = false;
            fetch('/stopstream')
                .then(() => {
                    streamImg.style.display = 'none';
                    streamImg.src = '';
                    streamPlaceholder.style.display = 'flex';
                    streamToggle.innerHTML = '<span class="material-icons">play_arrow</span>';
                    streamToggle.classList.remove('active');
                    showStatus('Stream stopped');
                })
                .catch(error => {
                    showStatus('Failed to stop stream: ' + error, true);
                });
        }

        // Joystick control
        function sendJoystickData(x, y, force = false) {
            // Не отправляем, если BT активен
            if (isBtActive) return;
            
            const now = Date.now();
            
            // Clear any pending timeout
            if (pendingJoystickSend) {
                clearTimeout(pendingJoystickSend.timeout);
            }

            // If forced or enough time has passed, send immediately
            if (force || now - lastJoystickSendTime >= THROTTLE_MS) {
                fetch('/control', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ 
                        mode: 'joystick',
                        x: x,
                        y: y
                    })
                }).catch(error => {
                    console.error('Failed to send control data:', error);
                });
                lastJoystickSendTime = now;
                pendingJoystickSend = null;
            } else {
                // Schedule to send at next available time
                pendingJoystickSend = {
                    data: { x, y },
                    timeout: setTimeout(() => {
                        sendJoystickData(x, y, true);
                    }, THROTTLE_MS - (now - lastJoystickSendTime))
                };
            }
        }

        function handleJoystickMove(event) {
            if (!isDragging) return;

            const rect = joystick.getBoundingClientRect();
            const centerX = rect.width / 2;
            const centerY = rect.height / 2;
            
            // Get touch or mouse position
            const clientX = event.clientX || event.touches[0].clientX;
            const clientY = event.clientY || event.touches[0].clientY;

            // Calculate relative position from center
            let x = clientX - rect.left - centerX;
            let y = clientY - rect.top - centerY;

            // Limit to square boundary
            const maxOffset = rect.width / 2 - stick.offsetWidth / 2;
            x = Math.max(-maxOffset, Math.min(maxOffset, x));
            y = Math.max(-maxOffset, Math.min(maxOffset, y));

            // Update stick position
            stick.style.transform = `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`;

            // Calculate normalized values (-1 to 1)
            const normalizedX = x / maxOffset;
            const normalizedY = -y / maxOffset; // Invert Y for traditional control scheme

            // Only send if values changed significantly
            if (Math.abs(normalizedX - currentX) > 0.1 || Math.abs(normalizedY - currentY) > 0.1) {
                currentX = normalizedX;
                currentY = normalizedY;
                sendJoystickData(normalizedX, normalizedY);
            }
        }

        function resetJoystick(sendData = false) {
            isDragging = false;
            stick.style.transform = 'translate(-50%, -50%)';
            currentX = 0;
            currentY = 0;
            // Отправляем данные только если это явно запрошено
            if (sendData && isJoystickMode && !isBtActive) {
                sendJoystickData(0, 0, true);
            }
        }

        // Mouse events for joystick
        stick.addEventListener('mousedown', (e) => {
            e.preventDefault();
            isDragging = true;
        });
        
        document.addEventListener('mousemove', (e) => {
            e.preventDefault();
            handleJoystickMove(e);
        });
        
        document.addEventListener('mouseup', (e) => {
            e.preventDefault();
            resetJoystick(true);
        });

        // Touch events for joystick
        stick.addEventListener('touchstart', (e) => {
            e.preventDefault();
            isDragging = true;
            handleJoystickMove(e.touches[0]);
        }, { passive: false });
        
        document.addEventListener('touchmove', (e) => {
            e.preventDefault();
            if (isDragging && e.touches.length > 0) {
                handleJoystickMove(e.touches[0]);
            }
        }, { passive: false });
        
        document.addEventListener('touchend', (e) => {
            e.preventDefault();
            resetJoystick(true);
        }, { passive: false });

        document.addEventListener('touchcancel', (e) => {
            e.preventDefault();
            resetJoystick(true);
        }, { passive: false });

        // Обработчики для кнопок
        streamToggle.addEventListener('click', toggleStream);

        // Добавляем обработку тач-событий для кнопок
        [streamToggle, fullscreenButton, settingsToggle, btToggle, joystickTab, slidersTab].forEach(button => {
            if (button) {
                button.addEventListener('touchstart', (e) => {
                    e.preventDefault();
                    button.click(); // Эмулируем обычный клик
                }, { passive: false });
            }
        });

        // Предотвращаем скролл/зум на мобильных устройствах
        document.addEventListener('gesturestart', (e) => {
            e.preventDefault();
        }, { passive: false });

        document.addEventListener('gesturechange', (e) => {
            e.preventDefault();
        }, { passive: false });

        document.addEventListener('gestureend', (e) => {
            e.preventDefault();
        }, { passive: false });

        // Sliders control
        function sendSliderData(data, force = false) {
            // Не отправляем, если BT активен
            if (isBtActive) return;
            
            const now = Date.now();
            
            // Clear any pending timeout
            if (pendingSend) {
                clearTimeout(pendingSend.timeout);
            }

            // If forced or enough time has passed, send immediately
            if (force || now - lastSendTime >= THROTTLE_MS) {
                fetch('/control', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(data)
                }).catch(error => {
                    console.error('Failed to send sliders data:', error);
                });
                lastSendTime = now;
                pendingSend = null;
            } else {
                // Schedule to send at next available time
                pendingSend = {
                    data: data,
                    timeout: setTimeout(() => {
                        sendSliderData(data, true);
                    }, THROTTLE_MS - (now - lastSendTime))
                };
            }
        }

        function startDragging(e, thumb) {
            e.preventDefault();
            e.stopPropagation();
            
            // Обрабатываем все новые касания
            if (e.touches) {
                Array.from(e.changedTouches).forEach(touch => {
                    const rect = thumb.parentElement.getBoundingClientRect();
                    activeTouches.set(touch.identifier, {
                        thumb: thumb,
                        startY: touch.clientY,
                        rect: rect,
                        startTop: (parseFloat(thumb.style.top) || 50) / 100 * rect.height
                    });
                });
            } else {
                activeTouches.set('mouse', {
                    thumb: thumb,
                    startY: e.clientY,
                    rect: thumb.parentElement.getBoundingClientRect(),
                    startTop: (parseFloat(thumb.style.top) || 50) / 100 * thumb.parentElement.getBoundingClientRect().height
                });
            }
        }

        function handleDragging(e) {
            e.preventDefault();
            e.stopPropagation();

            // Обработка всех активных касаний
            if (e.touches) {
                Array.from(e.touches).forEach(touch => {
                    const touchData = activeTouches.get(touch.identifier);
                    if (touchData) {
                        updateThumbPosition(touch.clientY, touchData);
                    }
                });
            } else if (activeTouches.has('mouse')) {
                const touchData = activeTouches.get('mouse');
                updateThumbPosition(e.clientY, touchData);
            }
        }

        function updateThumbPosition(clientY, touchData) {
            const { thumb, startY, rect, startTop } = touchData;
            const deltaY = clientY - startY;
            
            // Вычисляем новую позицию
            let newTop = startTop + deltaY;
            
            // Ограничиваем движение
            const thumbSize = thumb.offsetHeight;
            const minTop = thumbSize / 2;
            const maxTop = rect.height - thumbSize / 2;
            newTop = Math.max(minTop, Math.min(maxTop, newTop));
            
            // Обновляем позицию в процентах
            thumb.style.top = (newTop / rect.height * 100) + '%';
            
            // Вычисляем значение для отправки
            const normalizedPosition = (newTop - minTop) / (maxTop - minTop);
            const value = -(normalizedPosition * 2 - 1);
            
            // Отправляем данные
            const data = {
                mode: 'sliders',
                left: thumb === leftThumb ? value : leftValue(),
                right: thumb === rightThumb ? value : rightValue()
            };
            
            sendSliderData(data);
        }

        function stopDragging(e) {
            e.preventDefault();
            e.stopPropagation();
            
            let releasedThumbs = new Set();
            
            if (e.changedTouches) {
                Array.from(e.changedTouches).forEach(touch => {
                    const touchData = activeTouches.get(touch.identifier);
                    if (touchData) {
                        releasedThumbs.add(touchData.thumb);
                        activeTouches.delete(touch.identifier);
                    }
                });
            } else {
                const touchData = activeTouches.get('mouse');
                if (touchData) {
                    releasedThumbs.add(touchData.thumb);
                    activeTouches.delete('mouse');
                }
            }
            
            // Сбрасываем позиции отпущенных слайдеров
            releasedThumbs.forEach(thumb => {
                thumb.style.top = '50%';
            });
            
            // Отправляем последнюю позицию
            if (activeTouches.size === 0) {
                const data = {
                    mode: 'sliders',
                    left: releasedThumbs.has(leftThumb) ? 0 : leftValue(),
                    right: releasedThumbs.has(rightThumb) ? 0 : rightValue()
                };
                sendSliderData(data, true);
            }
        }

        function initSliders() {
            [leftThumb, rightThumb].forEach(thumb => {
                // Mouse events
                thumb.addEventListener('mousedown', (e) => startDragging(e, thumb));
                
                // Touch events with passive: false
                thumb.addEventListener('touchstart', (e) => startDragging(e, thumb), { passive: false });
            });

            // Global events
            document.addEventListener('mousemove', handleDragging);
            document.addEventListener('touchmove', handleDragging, { passive: false });
            document.addEventListener('mouseup', stopDragging);
            document.addEventListener('touchend', stopDragging, { passive: false });
            document.addEventListener('touchcancel', stopDragging, { passive: false });
        }

        function leftValue() {
            const rect = leftThumb.parentElement.getBoundingClientRect();
            const thumbRect = leftThumb.getBoundingClientRect();
            const thumbSize = leftThumb.offsetHeight;
            const minTop = thumbSize / 2;
            const maxTop = rect.height - thumbSize / 2;
            const currentTop = thumbRect.top + thumbSize/2 - rect.top;
            const value = Math.max(-1, Math.min(1, -((currentTop - minTop) / (maxTop - minTop) * 2 - 1)));
            return value;
        }

        function rightValue() {
            const rect = rightThumb.parentElement.getBoundingClientRect();
            const thumbRect = rightThumb.getBoundingClientRect();
            const thumbSize = rightThumb.offsetHeight;
            const minTop = thumbSize / 2;
            const maxTop = rect.height - thumbSize / 2;
            const currentTop = thumbRect.top + thumbSize/2 - rect.top;
            const value = Math.max(-1, Math.min(1, -((currentTop - minTop) / (maxTop - minTop) * 2 - 1)));
            return value;
        }

        // Добавляем обработчик для кнопки полноэкранного режима
        function toggleFullscreen() {
            if (!document.fullscreenElement) {
                document.documentElement.requestFullscreen().catch(err => {
                    showStatus('Error attempting to enable fullscreen: ' + err.message, true);
                });
            } else {
                document.exitFullscreen();
            }
        }

        fullscreenButton.addEventListener('click', toggleFullscreen);
        fullscreenButton.addEventListener('touchstart', (e) => {
            e.preventDefault();
            toggleFullscreen();
        }, { passive: false });

        // Initialize controls - не отправляем запрос при инициализации
        resetJoystick(false);
        initSliders();

        // Назначаем обработчик для кнопки включения/выключения Bluetooth
        btToggle.addEventListener('click', toggleBluetooth);
        
        // Запускаем периодический опрос статуса Bluetooth
        checkBluetoothStatus();

        function checkBluetoothStatus() {
            fetch('/bt/status')
                .then(response => response.json())
                .then(data => {
                    const btStatusElement = document.querySelector('.bt-status');
                    btStatusElement.textContent = data.status;
                    
                    // Проверяем статус подключения
                    const isConnected = data.connected;
                    
                    // Если статус изменился
                    if (isConnected !== btConnected) {
                        btConnected = isConnected;
                        
                        // Изменяем интервал опроса в зависимости от статуса подключения
                        btPollingInterval = btConnected ? 5000 : 2000; // 5 секунд если подключено, 2 секунды если нет
                    }
                    
                    if (data.enabled) {
                        btStatusElement.classList.remove('status-disconnected');
                        btStatusElement.classList.add('status-connected');
                        btToggle.classList.add('active');
                        isBtActive = true;
                        
                        // Скрываем элементы управления напрямую
                        const joystickContainer = document.querySelector('.joystick-container');
                        const slidersContainer = document.querySelector('.sliders-container');
                        if (joystickContainer) joystickContainer.style.display = 'none';
                        if (slidersContainer) slidersContainer.style.display = 'none';
                        
                        // Показываем обе вкладки как активные при активном BT
                        joystickTab.classList.add('active');
                        slidersTab.classList.add('active');
                    } else {
                        btStatusElement.classList.remove('status-connected');
                        btStatusElement.classList.add('status-disconnected');
                        btToggle.classList.remove('active');
                        isBtActive = false;
                        
                        // Обновляем выбранную вкладку
                        joystickTab.classList.toggle('active', isJoystickMode);
                        slidersTab.classList.toggle('active', !isJoystickMode);
                        
                        // Показываем соответствующие элементы управления напрямую
                        const joystickContainer = document.querySelector('.joystick-container');
                        const slidersContainer = document.querySelector('.sliders-container');
                        
                        if (isJoystickMode) {
                            if (joystickContainer) joystickContainer.style.display = 'block';
                            if (slidersContainer) slidersContainer.style.display = 'none';
                            joystickControl.classList.add('active');
                            slidersControl.classList.remove('active');
                        } else {
                            if (joystickContainer) joystickContainer.style.display = 'none';
                            if (slidersContainer) slidersContainer.style.display = 'block';
                            joystickControl.classList.remove('active');
                            slidersControl.classList.add('active');
                        }
                    }
                    
                    updateButtonUI();
                })
                .catch(error => {
                    console.error('Ошибка при получении статуса Bluetooth:', error);
                })
                .finally(() => {
                    // Устанавливаем следующий опрос с текущим интервалом
                    setTimeout(checkBluetoothStatus, btPollingInterval);
                });
        }

        // Функция для переключения Bluetooth
        function toggleBluetooth() {
            // Определяем текущий режим управления
            const currentMode = isJoystickMode ? 'joystick' : 'sliders';
            const action = isBtActive ? 'off' : 'on';
            
            fetch(`/bt?state=${action}&mode=${currentMode}`)
                .then(response => {
                    if (response.ok) {
                        // Устанавливаем новое состояние BT
                        isBtActive = !isBtActive;
                        
                        // Непосредственно обновляем UI элементов
                        if (isBtActive) {
                            // Скрываем элементы управления сразу при включении BT
                            const joystickContainer = document.querySelector('.joystick-container');
                            const slidersContainer = document.querySelector('.sliders-container');
                            
                            if (joystickContainer) joystickContainer.style.display = 'none';
                            if (slidersContainer) slidersContainer.style.display = 'none';
                            
                            document.getElementById('control-mode').classList.add('active');
                        } else {
                            // Показываем соответствующие элементы управления при выключении BT
                            const joystickContainer = document.querySelector('.joystick-container');
                            const slidersContainer = document.querySelector('.sliders-container');
                            const controlMode = document.getElementById('control-mode');
                            
                            controlMode.textContent = isJoystickMode ? 'Joystick' : 'Sliders';
                            
                            if (isJoystickMode) {
                                if (joystickContainer) joystickContainer.style.display = 'block';
                                if (slidersContainer) slidersContainer.style.display = 'none';
                                joystickControl.classList.add('active');
                                slidersControl.classList.remove('active');
                            } else {
                                if (joystickContainer) joystickContainer.style.display = 'none';
                                if (slidersContainer) slidersContainer.style.display = 'block';
                                joystickControl.classList.remove('active');
                                slidersControl.classList.add('active');
                            }
                        }
                        
                        // Обновляем кнопку BT
                        updateButtonUI();
                        
                        // После включения/выключения сразу проверяем статус
                        setTimeout(checkBluetoothStatus, 500);
                    }
                })
                .catch(error => {
                    console.error(`Ошибка при ${action === 'on' ? 'включении' : 'выключении'} Bluetooth:`, error);
                });
        }

        // Обновление UI кнопок в зависимости от состояния Bluetooth
        function updateButtonUI() {
            if (isBtActive) {
                btToggle.classList.add('active');
                btToggle.textContent = 'Выключить BT';
                btStatus.classList.add('active');
            } else {
                btToggle.classList.remove('active');
                btToggle.textContent = 'Включить BT';
                btStatus.classList.remove('active');
                btStatus.textContent = 'Bluetooth: Выключен';
            }
        }

        // Функция для сброса слайдеров без отправки данных
        function resetSlidersVisual() {
            leftThumb.style.top = '50%';
            rightThumb.style.top = '50%';
        }

        // Predefined event handler for initializeSettings
        function initializeSettings() {
            // Get camera settings
            fetch('/camera/settings')
                .then(response => response.json())
                .then(data => {
                    // Update resolution select
                    if (data.resolution) {
                        const option = Array.from(resolutionSelect.options).find(opt => opt.value === data.resolution);
                        if (option) {
                            resolutionSelect.value = data.resolution;
                        }
                    }
                    
                    // Update quality slider
                    if (data.quality) {
                        qualitySlider.value = data.quality;
                        qualityValue.textContent = data.quality + 
                            (data.quality < 30 ? " (Higher quality)" : " (Lower quality)");
                    }
                    
                    // Set initial LED value
                    const initialLedValue = 0; // Default to off
                    ledSlider.value = initialLedValue;
                    updateLedValue(initialLedValue);
                })
                .catch(error => {
                    console.error('Failed to get camera settings:', error);
                });
        }

        // Call initialization on page load
        initializeSettings();

        // LED slider handling
        ledSlider.addEventListener('input', function() {
            const value = parseInt(this.value);
            updateLedValue(value);
        });

        ledSlider.addEventListener('change', function() {
            const value = parseInt(this.value);
            updateLedValue(value);
            sendLedValue(value);
        });

        function updateLedValue(value) {
            let status = 'Off';
            
            if (value > 0 && value <= 50) {
                status = value + '% (Mid)';
            } else if (value > 50) {
                status = value + '% (High)';
            }
            
            ledValue.textContent = 'LED: ' + status;
        }

        function sendLedValue(value) {
            let state = 'off';
            
            if (value > 0 && value <= 50) {
                state = 'mid';
            } else if (value > 50) {
                state = 'high';
            }
            
            fetch('/led?state=' + state)
                .then(response => response.text())
                .then(result => {
                    showStatus('LED brightness set to ' + ledValue.textContent);
                })
                .catch(error => {
                    showStatus('Failed to control LED: ' + error, true);
                });
        }

        // Control mode tabs handling
        joystickTab.addEventListener('click', function() {
            if (!isJoystickMode) {
                toggleControlMode('joystick');
            }
        });

        slidersTab.addEventListener('click', function() {
            if (isJoystickMode) {
                toggleControlMode('sliders');
            }
        });

        function toggleControlMode(mode) {
            isJoystickMode = mode === 'joystick';
            
            // Update tabs UI
            joystickTab.classList.toggle('active', isJoystickMode);
            slidersTab.classList.toggle('active', !isJoystickMode);
            
            // Update control containers
            if (isJoystickMode) {
                joystickControl.classList.add('active');
                slidersControl.classList.remove('active');
            } else {
                joystickControl.classList.remove('active');
                slidersControl.classList.add('active');
            }
            
            // Update visible controls if not in BT mode
            if (!isBtActive) {
                const joystickContainer = document.querySelector('.joystick-container');
                const slidersContainer = document.querySelector('.sliders-container');
                
                if (isJoystickMode) {
                    if (joystickContainer) joystickContainer.style.display = 'block';
                    if (slidersContainer) slidersContainer.style.display = 'none';
                } else {
                    if (joystickContainer) joystickContainer.style.display = 'none';
                    if (slidersContainer) slidersContainer.style.display = 'block';
                }
            }
            
            // Reset control positions without sending data
            if (isJoystickMode) {
                isDragging = false;
                stick.style.transform = 'translate(-50%, -50%)';
                currentX = 0;
                currentY = 0;
            } else {
                leftThumb.style.top = '50%';
                rightThumb.style.top = '50%';
            }
            
            // Send mode change
            fetch('/control', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ mode: mode })
            }).then(response => {
                if (isBtActive && response.ok) {
                    setTimeout(checkBluetoothStatus, 500);
                }
            }).catch(error => {
                console.error('Failed to change control mode:', error);
            });
        }

        // Add custom slider touch handling
        function setupCustomSlider(sliderElement, valueElement, updateCallback, changeCallback) {
            let isActive = false;
            let activeTouch = null;
            
            function getValueFromPosition(clientX) {
                const rect = sliderElement.getBoundingClientRect();
                let percentage = (clientX - rect.left) / rect.width;
                percentage = Math.max(0, Math.min(1, percentage));
                
                const min = parseInt(sliderElement.min);
                const max = parseInt(sliderElement.max);
                return Math.round(min + percentage * (max - min));
            }
            
            function updateSlider(clientX) {
                const value = getValueFromPosition(clientX);
                sliderElement.value = value;
                updateCallback(value);
            }
            
            sliderElement.addEventListener('mousedown', function(e) {
                isActive = true;
                updateSlider(e.clientX);
                e.preventDefault();
            }, { passive: false });
            
            document.addEventListener('mousemove', function(e) {
                if (isActive) {
                    updateSlider(e.clientX);
                    e.preventDefault();
                }
            }, { passive: false });
            
            document.addEventListener('mouseup', function(e) {
                if (isActive) {
                    isActive = false;
                    changeCallback(parseInt(sliderElement.value));
                }
            });
            
            sliderElement.addEventListener('touchstart', function(e) {
                if (e.touches.length > 0) {
                    isActive = true;
                    activeTouch = e.touches[0].identifier;
                    updateSlider(e.touches[0].clientX);
                }
                e.preventDefault();
            }, { passive: false });
            
            document.addEventListener('touchmove', function(e) {
                if (!isActive) return;
                
                // Find our touch
                for (let i = 0; i < e.touches.length; i++) {
                    const touch = e.touches[i];
                    if (touch.identifier === activeTouch) {
                        updateSlider(touch.clientX);
                        e.preventDefault();
                        break;
                    }
                }
            }, { passive: false });
            
            function endTouch() {
                if (isActive) {
                    isActive = false;
                    activeTouch = null;
                    changeCallback(parseInt(sliderElement.value));
                }
            }
            
            document.addEventListener('touchend', endTouch);
            document.addEventListener('touchcancel', endTouch);
        }

        // Setup the quality slider
        setupCustomSlider(
            qualitySlider,
            qualityValue,
            function(value) {
                qualityValue.textContent = value + (value < 30 ? " (Higher quality)" : " (Lower quality)");
            },
            function(value) {
                sendQualitySettings();
            }
        );

        // Setup the LED slider
        setupCustomSlider(
            ledSlider,
            ledValue,
            function(value) {
                updateLedValue(value);
            },
            function(value) {
                sendLedValue(value);
            }
        );
    </script>
</body>
</html>)rawliteral";
