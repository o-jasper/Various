gcc en-morse.c -o en-morse

gcc en-morse-sound.c -o en-morse-sound -lSDL -lSDL_mixer -std=c99
gcc morse-sound.c -o morse-sound -lSDL -lSDL_mixer -std=c99
