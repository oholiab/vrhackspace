default: main

main.o: main.cpp
	g++ -c $<

main: main.o
	g++ main.o -o main -lIrrlicht

clean: main
	rm -rf $<
