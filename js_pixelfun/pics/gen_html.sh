#!/bin/bash

echo '<a href="http://ojasper.nl//blog/programs/2016/09/22/pixelfun.html">Back to blog post.</a>' > index.html 


ls *.png | while read line; do
    echo '<img src="'$line'">';
done >> index.html
