#!/bin/bash
cd $(dirname $1)
Xvfb :1 -screen 0 1024x768x16 &
outfile=$(basename $1)
tmpfile=tmpshot.png
sleep 3s
DISPLAY=:1 xterm &

while true; do
  sleep 1s
  DISPLAY=:1 scrot $tmpfile
  ln -s -f $tmpfile $outfile
done
