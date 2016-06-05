#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <fstream>
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
	FileSystem(char* fs_path);

	static int create(char* fs_path, size_t size);
	size_t freeSpace();

private:
	bool descriptorGood();
	int writeDescriptor();
	int readDescriptor();
	int initSpace(size_t size);
};

#endif