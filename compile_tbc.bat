del version.h
echo #define VANILLA 0 >> version.h
echo #define TBC 1 >> version.h
echo #define WOTLK 2 >> version.h
echo #define CATA 3 >> version.h
echo #define EXPANSION TBC >> version.h

gcc -c main.c
windres resource.rc resource.o
gcc -o "WoW Launcher.exe" main.o resource.o -mwindows