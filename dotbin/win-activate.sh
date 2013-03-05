#!/bin/bash
#Cyles through the windows of the list of comma-separated programs in $1.
# If none of the programs has a window, attempts to run the first one.

mkdir -p /tmp/win-activate/
I=`cat /tmp/win-activate/$1` #Contains index of current window.
if [ "$I" == "" ]; then
    I=1
else
    I=`expr $I + 1`
fi
echo $I > /tmp/win-activate/$1 #Update the index.

TO_WIN=`win-of-name.sh $1 | tail -n+$I | head -n 1`

echo $I
echo `win-of-name.sh $1 | tail -n+$I | head -n 1`

if [ "$TO_WIN" == "" ] ; then #Couldnt find a window.
    echo 1 > /tmp/win-activate/$1 #Try first one.
    TO_WIN=`win-of-name.sh $1 | head -n 1`
    if [ "$TO_WIN" == "" ] ; then #Couldnt find a window, try run first of list.
        `echo $1 | cut -f 1 -d ','`
    else #Cycle list.
        wmctrl -i -a $TO_WIN
    fi
else
    wmctrl -i -a $TO_WIN
fi
