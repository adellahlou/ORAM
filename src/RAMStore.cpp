#include "RAMStore.hpp"
	
RAMStore::RAMStore(size_t size)
: store(size)
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

size_t RAMStore::GetSize()
{
	return store.size();
}
