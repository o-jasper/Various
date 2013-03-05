#!/bin/bash

ffmpeg -f alsa -i default -f v4l2 -s 640x480 -i /dev/video0 -acodec flac -vcodec libx264 ~/pics/from-cam/`date +%s`.mkv
