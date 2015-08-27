#pragma once

#include "BlockStore.hpp"

class RAMStore: public BlockStore {
	std::vector<bytes<BlockSize>> store;

public:
	RAMStore(size_t size);
	~RAMStore();

	block Read(size_t pos);
	void Write(size_t pos, block b);

	size_t GetSize();
};
