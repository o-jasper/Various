all: prog-select
default: all

clean: 
	rm prog-select

pkg-config-store:  # Store, so dependency not always needed.
	pkg-config --cflags --libs gtk+-2.0 > pkg-config-store

prog-select: prog-select.c pkg-config-store
	gcc prog-select.c -o prog-select `cat pkg-config-store`
