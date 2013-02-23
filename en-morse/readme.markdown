
# Morse code stuff

Stuff to read and sound morse codes.

Compile using `make`

### en-morse

Turns text into '.' and '_', representing morse code.

With no arguments, it reads from stdin, otherwise from given file.

### de-morse

Does the reverse, conspicuously **not yet implemented**.

### morse-sound, en-morse-sound

The first one uses already `en-morse`d stuff, the latter can take text.

Uses SDL sound to make the sound, defaultly using
tick.wav and longtick.wav as sounds, but they can be replaced. Arguments are:

1. delay between letters

2. input morse file, text file respectively for morse-sound, 
en-morse-sound (overriding stdin)

3. override short .wav sound file. Default "tick.wav", you have to provide it.

4. override long .wav sound file. Default "longtick.wav", you have to provide
   it.

5. short-delay 

6. long delay.

### TODO
I find the project is probably a bit of a waste of time, so these won't happen
by my hand.

* get default tick.wav and longtick.wav with acceptable licenses.
(git is currently unaware of these) if you run linux with a lot installed you
can try look under `/usr/share/sounds/` and get them there, For instance 
`/usr/share/sounds/gnome/default/alerts/` .. `drip.ogg` and `glass.ogg` can be
converted to wav and used. (`ffmpeg -i infile outfile` will recognize 
extensions)

* potential issue, sometimes you have to refer to SDL as SDL/SDL.h sometimes 
  just SDL.h. (my case the first)

* make programs respond to '-h'.

* Make decoder.(and test)

* Default interval is a bit long.

* Didnt put any effort in checking it is right.

* Communicating efficiently to stuff like the arduino. 
  Interfacing with [this](http://arduino.cc/playground/Code/Morse).

### Author

Jasper den Ouden

placed in public domain
