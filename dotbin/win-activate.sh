#!/bin/bash
#Cyles through the windows of the list of comma-separated programs in $1.
# If none of the programs has a window, attempts to run the first one.

CAT_NAME=$1
WIN_LIST=$2
if [ "$2" == "" ]; then
    WIN_LIST="$CAT_NAME" #window list doubles as category name.
    echo > /dev/null
fi
EXEC_IF_NONE=$3

mkdir -p /tmp/win-activate/
I=`cat /tmp/win-activate/$CAT_NAME` #Contains index of current window.
if [ "$I" == "" ]; then
    I=1
else
    I=`expr $I + 1`
fi
echo $I > /tmp/win-activate/$CAT_NAME #Update the index.

TO_WIN=`win-of-name.sh $WIN_LIST | tail -n+$I | head -n 1`

if [ "$TO_WIN" == "" ]; then #Couldnt find a window.
    echo 1 > /tmp/win-activate/$CAT_NAME #Try first one.
    TO_WIN=`win-of-name.sh $WIN_LIST | head -n 1`
    if [ "$TO_WIN" == "" ]; then #Couldnt find a window, try run first of list.
        if [ "$EXEC_IF_NONE" == "" ]; then #First of lists is runnable.
            `echo $WIN_LIST | cut -f 1 -d ','`
        else
            $EXEC_IF_NONE
        fi
    else #Cycle list.
        wmctrl -i -a $TO_WIN
    fi
else
    wmctrl -i -a $TO_WIN
fi
