CPPFLAGS = -O2 --std=c++11 -Wall

all: virtual_fs

main.o: main.cpp
	g++ -c $(CPPFLAGS) main.cpp

virtual_fs: main.o
	g++ -o fs main.o

clean:
	rm *.o
	rm fs
