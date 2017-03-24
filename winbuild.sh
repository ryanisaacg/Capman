#!/bin/sh
make clean
LFLAGS=-Wl,-subsystem,windows-Wl,-subsystem,windows make
cp main.out main.exe
7z a -tzip winbuild.zip main.exe *.ttf *.wav *.ogg tracks levels *.lvl *.dll
rm main.exe
