#!/bin/bash
# WARNING this actually sucks! `xsel` could output something that executes arbitrary code!!

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

mpv --fullscreen --force-window $VID

mpc play
