import subprocess
import os


def main():
    subprocess.run(["scons", "--menuconfig"])
    # os.system("arm-none-eabi-objdump -d ./rtthread-stm32f4xx.elf >./rtthread.s")
    # os.system("/Users/huanghuisheng/.env/tools/scripts/pkgs ~/.env/env.sh")
    subprocess.run(["echo", "更新env......"])
    os.system("/Users/huanghuisheng/.env/tools/scripts/pkgs --upgrade")
    subprocess.run(["echo", "升级组件包......"])
    os.system("/Users/huanghuisheng/.env/tools/scripts/pkgs --update")
    subprocess.run(["scons", "--target=vsc", "-s"])
    subprocess.run(["echo", "尝试编译......."])
    subprocess.run(["python3", "./Link/rebuild.py"])


if __name__ == "__main__":
    main()
