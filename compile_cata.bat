del version.h
echo #define VANILLA 0 >> version.h
echo #define TBC 1 >> version.h
echo #define WOTLK 2 >> version.h
echo #define CATA 3 >> version.h
echo #define EXPANSION CATA >> version.h

gcc -c main.c
windres resource.rc resource.o
gcc -nostdlib -ffreestanding -mwindows -Os -fno-builtin-log -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -e_WinMainCRTStartup -o "WoW Launcher.exe" main.o resource.o -lkernel32 -luser32 -lshell32 -lgdi32
