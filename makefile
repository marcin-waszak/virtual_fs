CPPFLAGS = -O2 --std=c++11 -Wall
CPPFLAGSD = -g --std=c++11 -Wall

all: virtual_fs

main.o: main.cpp
	g++ -c $(CPPFLAGS) main.cpp
	
FileSystem.o: FileSystem.cpp
	g++ -c $(CPPFLAGS) FileSystem.cpp
	
maind.o: main.cpp
	g++ -c $(CPPFLAGSD) main.cpp -o maind.o
	
FileSystemd.o: FileSystem.cpp
	g++ -c $(CPPFLAGSD) FileSystem.cpp -o FileSystemd.o

virtual_fs: FileSystem.o main.o
	g++ -o fs FileSystem.o main.o

debug: FileSystemd.o maind.o
	g++ -g -o fsd FileSystemd.o maind.o
	gdb fsd

clean:
	rm *.o
	rm fs
