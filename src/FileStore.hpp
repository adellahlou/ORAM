#pragma once

#include "BlockStore.hpp"

class FileStore: public BlockStore {
	int fd;

	byte_t *map;
	size_t num, size;

public:
	FileStore(std::string filename, size_t num, size_t size);
	~FileStore();

	block Read(size_t pos);
	void Write(size_t pos, block b);

	size_t GetNum();
	size_t GetSize();
};
