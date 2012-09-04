#!/bin/bash
gcc prog-select.c -o prog-select `pkg-config --cflags --libs gtk+-2.0`
