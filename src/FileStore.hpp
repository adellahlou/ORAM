#pragma once

#include "BlockStore.hpp"

class FileStore: public BlockStore {
	int fd;

	byte_t *map;
	size_t size;

public:
	FileStore(std::string filename, size_t size);
	~FileStore();

	block Read(size_t pos);
	void Write(size_t pos, block b);

	size_t GetSize();
};
