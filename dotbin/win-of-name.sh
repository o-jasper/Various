#!/bin/bash
#Finds the windows of the comma-separated programs.

for el in `echo $1 | replace ',' '
'`; do
    AWK_FUN="{if(\$4==\"$el\"){print(\$1)}}"
    GOT_PID=`ps -U $USER -u $USER |grep -v "<defunct>" | awk $AWK_FUN`
#TODO instead of |head -n 1 use /tmp file to store the previous, and cycle the
#  particular set.
    for sel in `echo $GOT_PID`; do
        wmctrl -l -p | grep $sel        
    done
done
