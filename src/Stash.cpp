#include "Stash.hpp"
#include "File.hpp"

#include <fstream>
#include <cstring>

bool StashHelper::Load(std::string filename, Stash &stash)
{
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::binary);
	
	if (!file.good()) {
		return false;
	}
	
	size_t length = File::GetLength(file);
	
	// Does length correctly align?
	if (length % (sizeof (int) + CHUNK)) {
		return false;
	}
	
	for (size_t i = 0; i < length; i += sizeof (int) + CHUNK) {
		int id;
		Chunk chunk;
		
		File::Read(file, (raw_t *) &id, sizeof (int));
		File::Read(file, chunk.data(), chunk.size());
		
		stash[id] = chunk;
	}
	
	file.close();
	
	return true;
}


void StashHelper::Save(std::string filename, Stash &stash)
{
	std::fstream file;
	file.open(filename, std::fstream::out | std::fstream::binary | std::fstream::trunc);
	
	for (auto b : stash) {
		raw<sizeof (int) + CHUNK> plaintext;
		
		// Copy data to slab
		memcpy(plaintext.data(), &b.first, sizeof (int));
		memcpy(plaintext.data() + sizeof (int), &b.second, CHUNK);
		
		File::Write(file, plaintext.data(), plaintext.size());
	}
	
	file.close();
}
