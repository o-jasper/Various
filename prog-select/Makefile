all: prog-select
default: all

clean: 
	rm prog-select

prog-select: prog-select.c
	gcc prog-select.c -o prog-select `pkg-config --cflags --libs gtk+-2.0`
