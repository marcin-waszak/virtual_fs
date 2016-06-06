#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <fstream>
#include <map>
#include <cstring>
#include <iostream>//

using namespace std;

class FileSystem {
public:
	static const size_t FORMAT_IDENTIFIER = 0x0053464D; // 'MFS' at the beginning
	static const size_t DEF_DATAOFFSET = 1024;
	static const size_t MIN_SIZE = 4 * DEF_DATAOFFSET;

private:
	// Filesystem descriptor struct
	struct fsdesc_t {
		size_t format_id;
		size_t size;
		size_t n_files;
		size_t offset_data;
	};

	// File descriptor struct
	struct filedesc_t {
		char name[32];
		size_t offset_data;
		size_t data_size;
	};

	fstream fs_file;
	fsdesc_t fs_desc;
public:
	map<size_t, size_t> map_space;

	FileSystem(char* fs_path);

	int import(char* f_local, char* f_virtual);
	size_t freeSpace();
	size_t fileFind(char* fname);
	static bool checkIntersect(size_t start, size_t end, size_t position);
	static size_t fend();
	static int create(char* fs_path, size_t size);
	static size_t localFileSize(fstream& file);

private:
	bool descriptorGood();
	int writeDescriptor();
	int readDescriptor();
	int initSpace(size_t size);
	size_t descriptorsEnd();
	size_t findSpace(size_t space);
	bool checkBordersInInterval(size_t start, size_t end);
	int loadFileMap();
};

#endif