
# Bunch of stuff in by `~/.bin` directory
I have added  to `PWD=$PWD:~/.bin/` to `~/.bashrc` of course. Some of these are
attached to key bindings. How to set those depends on your windowing system.

## win-of-name.sh
Uses [`wmctrl`](http://tomas.styblo.name/wmctrl/) and `ps` to figure out which 
windows belong to a program or comma-separated list of programs. For instance:

    win-of-name.sh firefox,epiphany,netsurf,opera,liferea

Will get all the entries from `wmctrl -l -p` relevant to those. 
    
    wmctrl -i -a `win-of-name.sh firefox` #To open the first one.

## win-activate.sh
Also takes a comma-separated list of program names, but cycles through the
windows. If none of the programs in the list has a window, attempts to run the
first element of the (coma-separated)list.

    #Cycle or otherwise start firefox.
    win-activate.sh firefox,epiphany,netsurf,opera,liferea 

### What i use of those two:
Basically i have the following bound into `~/.fluxbox/keys`. Presumably the
lists will lengthen a bit.

    Mod4 z :Exec win-activate.sh emacs,openscad,gimp
    Mod4 a :Exec win-activate.sh firefox,opera,epiphany,liferea
    
    Mod4 c :Exec win-activate.sh gnome-terminal
    Mod4 v :Exec win-activate.sh claws-mail
    Mod4 s :Exec win-activate.sh ~/.bin/more/focus-evince.sh
    Mod4 x :Exec win-activate.sh gmpc,totem

## screenshot.sh
Immediately takes a screenshot, meant to be used as key binding.

The first argument can be nothing, then `zenity` is used to
ask for a name, or `date`, then the date is used for name, or it can be the name
itself.

Puts the screenshots in `~/pics/screens/`.

## video.sh
Starts using /dev/video0 to record into `~/pics/from-cam/`.

## video-pic.sh
Runs `mplayer` in order make pictures.(you have to trigger it) Puts it in
`~/pics/from-cam/`.

I suppose i should want to immediately take a picture.