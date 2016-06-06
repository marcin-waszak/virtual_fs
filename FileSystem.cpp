#include "FileSystem.hpp"

FileSystem::FileSystem(char* fs_path) {
	fs_file.open(fs_path, ios::in | ios::out | ios::binary);

	if(!fs_file.good())
		throw 1;

	readDescriptor();
	loadFileMap();
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
	fs_file.seekp(0, ios_base::beg);
	fs_file.write(reinterpret_cast<const char*>(&fs_desc), sizeof(fs_desc));

	return 0;
}

int FileSystem::writeDescriptor() {
	fs_file.seekp(0, ios_base::beg);
	fs_file.write(reinterpret_cast<const char*>(&fs_desc), sizeof(fs_desc));

	if(fs_file.fail())
		return 1;

	return 0;
}

int FileSystem::readDescriptor() {
	fs_file.seekg(0, ios_base::beg);
	fs_file.read(reinterpret_cast<char*>(&fs_desc), sizeof(fs_desc));

	if(fs_file.fail())
		return 1;

	return 0;
}


int FileSystem::import(char* f_local, char* f_virtual) {
	fstream file_local(f_local, ios::in | ios::binary);

	if(!file_local.good())
		return 1;

	if(fileFind(f_virtual))
		return 1;
	size_t local_file_size = localFileSize(file_local);

	if(local_file_size > freeSpace())
		return 1;

	size_t data_position;
	if(!(data_position = findSpace(local_file_size)))
		return 1;

	char* temp = new char[local_file_size];
	file_local.seekg(0, ios_base::beg);
	file_local.read(temp, local_file_size);
	file_local.close();

	// define new file descriptor
	filedesc_t file_desc;
	strcpy(file_desc.name, f_virtual);
	file_desc.offset_data = data_position;
	file_desc.data_size = local_file_size;

	size_t position = descriptorsEnd();
	if(position == fend())
		return 1;

	// write file descriptor
	fs_file.seekp(position);
	fs_file.write(reinterpret_cast<char*>(&file_desc), sizeof(file_desc));

	// add new file descriptor to our fs
	fs_desc.n_files++;
	writeDescriptor();

	// write file data to fs
	fs_file.seekp(data_position);
	fs_file.write(reinterpret_cast<char*>(temp), local_file_size);

//	printf("Added in: $zu\n", )
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

size_t FileSystem::descriptorsEnd() {
	size_t descriptors_end;
	descriptors_end = sizeof(fsdesc_t) + fs_desc.n_files * sizeof(filedesc_t);

	if(descriptors_end + sizeof(filedesc_t) > DEF_DATAOFFSET)
		return fend();

	return descriptors_end;
}

size_t FileSystem::localFileSize(fstream& file) {
	file.seekg(0, ios_base::end);
	return file.tellg();
}

size_t FileSystem::fileFind(char* fname) {
	char buffer[32];

	for(size_t i = 0; i < fs_desc.n_files; i++) {
		size_t position = sizeof(fsdesc_t) + i * sizeof(filedesc_t);

		fs_file.seekg(position);
		fs_file.read(reinterpret_cast<char*>(buffer), 32);

		if(strcmp(fname, buffer) == 0)
			return position;
	}

	return 0;
}

size_t FileSystem::findSpace(size_t space) {	
	size_t where = 0;

	size_t a = fs_desc.offset_data;
	size_t b = fs_desc.offset_data + space - 1;

	if(!checkBordersInInterval(a, b))
		where = a;

	for(auto &i : map_space) {
		if(where)
			break;

		size_t start = i.first + i.second;
		size_t end = i.first + i.second + space - 1;

		if(!checkBordersInInterval(start, end))
			if(end < fs_desc.size)
				where = start;
	}

	if(!where)
		return 0;

	map_space[where] = space;
	return where;
}

bool FileSystem::checkBordersInInterval(size_t start, size_t end) {
	for(auto &i : map_space) {
		size_t a = i.first;
		size_t b = i.first + i.second - 1;

		if(checkIntersect(start, end, a))
			return true;

		if(checkIntersect(start, end, b))
			return true;
	}

	return false;
}

bool FileSystem::checkIntersect(size_t start, size_t end, size_t position) {
	if(position >= start && position <= end)
		return true;

	return false;
}

int FileSystem::loadFileMap() {
	filedesc_t descriptor;

	for(size_t i = 0; i < fs_desc.n_files; i++) {
		size_t position = sizeof(fsdesc_t) + i * sizeof(filedesc_t);

		fs_file.seekg(position);
		fs_file.read(reinterpret_cast<char*>(&descriptor), sizeof(filedesc_t));

		size_t d_offset = descriptor.offset_data;
		size_t d_size = descriptor.data_size;
		
		map_space[d_offset] = d_size;
	}

	return 0;
}

size_t FileSystem::fend() {
	return -1;
}