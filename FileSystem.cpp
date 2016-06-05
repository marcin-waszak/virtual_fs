#include "FileSystem.hpp"

FileSystem::FileSystem(char* fs_path) {
	fs_file.open(fs_path, ios::in | ios::out | ios::binary);

	if(!fs_file.good())
		throw 1;

	readDescriptor();
}

int FileSystem::create(char* fs_path, size_t size) {
	fstream fs_file(fs_path, ios::out | ios::binary);

	if(!fs_file.good())
		return 1;

	if(size < MIN_SIZE)
		return 1;

	// prepare initial fs descriptor
	fsdesc_t fs_desc;

	fs_desc.format_id = FORMAT_IDENTIFIER;
	fs_desc.size = size;
	fs_desc.n_files = 0;
	fs_desc.offset_data = DEF_DATAOFFSET;

	// resize fs file
	fs_file.seekp(size - 1);
	fs_file.write("", 1);

	// store fs descriptor into the file
	fs_file.seekp(ios_base::beg);
	fs_file.write(reinterpret_cast<const char*>(&fs_desc), sizeof(fs_desc));

	return 0;
}

int FileSystem::writeDescriptor() {
	fs_file.seekp(ios_base::beg);
	fs_file.write(reinterpret_cast<const char*>(&fs_desc), sizeof(fs_desc));

	if(fs_file.fail())
		return 1;

	return 0;
}

int FileSystem::readDescriptor() {
	fs_file.seekg(ios_base::beg);
	fs_file.read(reinterpret_cast<char*>(&fs_desc), sizeof(fs_desc));

	if(fs_file.fail())
		return 1;

	return 0;
}

size_t FileSystem::freeSpace() {
	size_t free_space = fs_desc.size - fs_desc.offset_data;
	return free_space;
}

bool FileSystem::descriptorGood() {
	if(fs_desc.format_id == FORMAT_IDENTIFIER)
		return true;

	return false;
}
