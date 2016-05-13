levelmodel=models/map-20kdm2.pk3
TARGETS=main termtest.out
CONTAINER?=vrhs

default: $(TARGETS)

termtest.out: termtest.cpp controlterm.cpp
	g++ $< -lX11 -o termtest.out

main.o: main.cpp controlterm.cpp
	g++ -c $<

$(levelmodel):
	mkdir -p models
	cd models && wget https://github.com/kexplo/irrlicht_ogl-es/raw/master/media/map-20kdm2.pk3

main: main.o $(levelmodel)
	g++ main.o -o main -lIrrlicht -lX11

clean: main
	rm -rf $<

image: Dockerfile screenme.sh
	docker build -t vrhs .

runc: image
	docker run -v /tmp/vrhs:/vrhs -v /tmp/.X11-unix:/tmp/.X11-unix -v $(shell pwd):/workspace --rm --name vrhs $(CONTAINER)

run: main
	./main | grep -v "Loaded texture"
