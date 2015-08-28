#include "RAMStore.hpp"
	
RAMStore::RAMStore(size_t num, size_t size)
: store(num), size(size)
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

size_t RAMStore::GetNum()
{
	return store.size();
}

size_t RAMStore::GetSize()
{
	return size;
}
