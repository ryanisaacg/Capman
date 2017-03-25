#!/bin/sh
make clean
make EXTRA-CFLAGS=-O3 EXTRA-LFLAGS=-Wl,-subsystem,windows-Wl,-subsystem,windows
cp main.out main.exe
7z a -tzip winbuild.zip main.exe *.ttf *.wav *.ogg tracks levels *.lvl *.dll *.png
rm main.exe
