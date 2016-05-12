#!/bin/bash
cd $(dirname $1)
Xvfb :1 -screen 0 1024x768x16 &
outfile=$(basename $1)
tmpfile1=tmpshot1.png
tmpfile2=tmpshot2.png
sleep 3s
DISPLAY=:1 stterm &
sleep 3s
DISPLAY=:1 xte 'mousemove 100 100'

while true; do
  if [ "$tmpfile" = "$tmpfile1" ]; then
    tmpfile=$tmpfile2
  else
    tmpfile=$tmpfile1
  fi
  sleep 0.01s
  DISPLAY=:1 scrot $tmpfile
  ln -s -f $tmpfile $outfile
done
