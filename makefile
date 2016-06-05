CPPFLAGS = -O2 --std=c++11 -Wall

all: virtual_fs

main.o: main.cpp
	g++ -c $(CPPFLAGS) main.cpp
	
FileSystem.o: FileSystem.cpp
	g++ -c $(CPPFLAGS) FileSystem.cpp

virtual_fs: FileSystem.o main.o
	g++ -o fs FileSystem.o main.o

clean:
	rm *.o
	rm fs
