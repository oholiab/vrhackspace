default: main

main.o: main.cpp
	g++ -c $<

main: main.o
	g++ main.o -o main -lIrrlicht

clean: main
	rm -rf $<

image:
	docker build -t roomtest .

run:
	docker run -v /tmp/roomtest:/roomtest --rm --name roomtest roomtest
