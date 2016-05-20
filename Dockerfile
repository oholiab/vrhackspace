FROM ubuntu:trusty
RUN apt-get update && apt-get -y install \
  libirrlicht1.8 \
  libirrlicht-dev \
  g++ \
  git \
  make \
  libx11-dev
RUN apt-get -y install make
RUN apt-get -y install libx11-dev
ADD src /workspace
WORKDIR /workspace
RUN make
CMD make run
