#!/bin/bash
export DISPLAY=$2
cd $(dirname $1)
Xvfb $DISPLAY -screen 0 1024x768x16 &
outfile=$(basename $1)
tmpfile1=tmpshot1.png
tmpfile2=tmpshot2.png
sleep 3s
stterm &
sleep 3s
xte 'mousemove 100 100'

while true; do
  if [ "$tmpfile" = "$tmpfile1" ]; then
    tmpfile=$tmpfile2
  else
    tmpfile=$tmpfile1
  fi
  scrot $tmpfile
  ln -s -f $tmpfile $outfile
done
