#!/bin/bash

FILE="/tmp/scrot_$RANDOM_`date +%s`.png"
scrot $FILE

if [ "$1" = "" ] ; then
    SCREENSHOT_NAME=`zenity --title "Screenshot name" --entry`;
elif [ "$1" = "date" ] ; then
    SCREENSHOT_NAME=`date +%Y-%m-%d:%H:%M:%S`;
elif [ "" = "" ] ; then #derp, shell script sucks
    SCREENSHOT_NAME="$1";
fi

mv $FILE ~/pics/screens/$SCREENSHOT_NAME.png
