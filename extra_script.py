import subprocess
import os
from SCons.Script import Import

Import("env")

def run_generate_build_flags(source, target, env):
    script_path = os.path.join(env['PROJECT_DIR'], 'generate_build_flags.sh')
    bash_path = r"C:\Program Files\Git\bin\bash.exe"
    try:
        result = subprocess.run([bash_path, script_path], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error executing generate_build_flags.sh: {e.stderr}")
        env.Exit(1)

env.AddPreAction("buildprog", run_generate_build_flags)
