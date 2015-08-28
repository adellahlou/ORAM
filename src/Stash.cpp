#include "Stash.hpp"
#include "File.hpp"

#include <fstream>
#include <cstring>

bool StashHelper::Load(std::string filename, Stash &stash)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	
	if (!file.good()) {
		return false;
	}
	
	size_t length = File::GetLength(file);
	
	// Does length correctly align?
	if (length % (sizeof (int) + ChunkSize)) {
		file.close();
		return false;
	}
	
	for (size_t i = 0; i < length; i += sizeof (int) + ChunkSize) {
		int id;
		Chunk chunk;
		
		File::Read(file, (byte_t *) &id, sizeof (int));
		File::Read(file, chunk.data(), chunk.size());
		
		stash[id] = chunk;
	}
	
	file.close();
	return true;
}


void StashHelper::Save(std::string filename, Stash &stash)
{
	std::fstream file;
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	
	for (auto b : stash) {
		bytes<sizeof (int) + ChunkSize> plaintext;
		
		// Copy data to slab
		memcpy(plaintext.data(), &b.first, sizeof (int));
		memcpy(plaintext.data() + sizeof (int), &b.second, ChunkSize);
		
		File::Write(file, plaintext.data(), plaintext.size());
	}
	
	file.close();
}
