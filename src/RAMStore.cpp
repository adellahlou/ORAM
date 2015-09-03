#include "RAMStore.hpp"
	
RAMStore::RAMStore(size_t count, size_t size)
: store(count), size(size)
{}

RAMStore::~RAMStore()
{}

block RAMStore::Read(size_t pos)
{
	return store[pos];
}

void RAMStore::Write(size_t pos, block b)
{
	store[pos] = b;
}

size_t RAMStore::GetBlockCount()
{
	return store.size();
}

size_t RAMStore::GetBlockSize()
{
	return size;
}
