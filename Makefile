levelmodel=models/map-20kdm2.pk3

default: main

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
	docker run -v /tmp/roomtest:/roomtest --rm --name roomtest roomtest
