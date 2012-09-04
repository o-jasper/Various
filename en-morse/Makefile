
all: en-morse en-morse-sound morse-sound
default: all

clean: 
	rm en-morse en-morse-sound morse-sound

en-morse: en-morse.c
	gcc en-morse.c -o en-morse
en-morse-sound: en-morse-sound.c sound.c morse-sound-include.c
	gcc en-morse-sound.c -o en-morse-sound -lSDL -lSDL_mixer -std=c99
morse-sound: morse-sound.c sound.c morse-sound-include.c
	gcc morse-sound.c -o morse-sound -lSDL -lSDL_mixer -std=c99
