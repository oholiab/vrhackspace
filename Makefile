levelmodel=models/map-20kdm2.pk3
TARGETS=main termtest.out

default: $(TARGETS)

termtest.out: termtest.cpp
	g++ $< -lX11 -o termtest.out

main.o: main.cpp
	g++ -c $<

$(levelmodel):
	mkdir -p models
	cd models && wget https://github.com/kexplo/irrlicht_ogl-es/raw/master/media/map-20kdm2.pk3

main: main.o $(levelmodel)
	g++ main.o -o main -lIrrlicht

clean: main
	rm -rf $<

image:
	docker build -t roomtest .

run:
	docker run -v /tmp/roomtest:/roomtest -v /tmp/.X11-unix:/tmp/.X11-unix --rm --name roomtest roomtest
