FROM ubuntu:trusty
RUN apt-get update && apt-get -y install xvfb xterm scrot
ADD screenme.sh /screenme.sh
CMD /screenme.sh /roomtest/shot.png
