#!/bin/bash

if [ $1 == "wait" ]; then
    mpc current --wait
fi

mpc pause
VID=$2
if [ $VID == "sel" ]; then
    VID=$(xsel -p)
elif [ "$VID" == 'clip' ]; then
    VID=$(xsel -b)
fi
#echo $VID >> /tmp/pause-during
DISPLAY=:2 lns-trick mpv mpv --fullscreen --force-window $VID

mpc play
