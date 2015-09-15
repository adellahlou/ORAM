#include "Agent.hpp"
#include "FileStore.hpp"
#include "File.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <cassert>

void error(const char *str)
{
	fputs(str, stderr);
	exit(1);
}

Agent::Agent(BlockStore *store, size_t count, size_t size, bytes<Key> key)
: store(store), count(count), size(size), key(key)
{
	size_t clen = IV + AES::GetCiphertextLength(GetBlockSize());
	
	if (store->GetBlockSize() != clen) {
		error("store block size is too small");
	}

	if (store->GetBlockCount() < count) {
		error("store has too few blocks");
	}
}

Agent::~Agent()
{
	delete store;
}

PositionMap Agent::LoadPositionMap()
{
	size_t num = store->GetBlockCount();

	std::fstream file;
	file.open("posmap.bin", std::ios::in | std::ios::binary);

	if (!file.good()) {
		PositionMap posMap(num);

		// All blocks are initally empty
		std::fill(posMap.begin(), posMap.end(), -1);

		return posMap;
	}

	size_t length = File::GetLength(file);

	if (length != num*sizeof (int64_t)) {
		file.close();
		
		PositionMap posMap(num);

		// All blocks are initally empty
		std::fill(posMap.begin(), posMap.end(), -1);

		return posMap;
	}

	PositionMap posMap(num);
	file.read((char *) posMap.data(), num*sizeof (int64_t));

	return posMap;
}

void SavePositionMap(PositionMap posMap)
{
	std::fstream file;
	file.open("posmap.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	
	file.write((char *) posMap.data(), posMap.size()*sizeof (int64_t));
	file.close();
}

BlockMap Agent::GenerateBlockMap(PositionMap posMap)
{
	BlockMap blockMap;

	for (size_t i = 0; i < posMap.size(); i++) {
		int64_t bid = posMap[i];
		blockMap[bid].push_back(i);
	}

	return blockMap;
}

ChangedMap Agent::LoadChangedMap()
{
	std::fstream file;
	file.open("changedmap.bin", std::ios::in | std::ios::binary);

	ChangedMap changedMap(count);

	if (!file.good()) {
		// Initially no blocks have been changed
		std::fill(changedMap.begin(), changedMap.end(), 0);
	} else {
		// Get from file
		file.read((char *) changedMap.data(), changedMap.size()*sizeof (byte_t));
	}

	return changedMap;
}

void Agent::SaveChangedMap(ChangedMap changedMap)
{
	std::fstream file;
	file.open("changedmap.bin", std::ios::out | std::ios::trunc | std::ios::binary);

	file.write((char *) changedMap.data(), changedMap.size()*sizeof (byte_t));
	file.close();
}

size_t GetFreePosition(PositionMap &posMap, BlockMap &blockMap)
{
	for (;;) {
		size_t randPos = rand() % posMap.size();
		int64_t bid = posMap[randPos];

		if (bid == -1) {
			return randPos;
		}

		auto &positions = blockMap[bid];

		// If the block is duplicated
		if (positions.size() > 1) {
			// Remove the position from maps
			posMap[randPos] = -1;
			std::remove(positions.begin(), positions.end(), randPos);

			return randPos;
		}
	}
}

size_t GetBlockPosition(int64_t bid, PositionMap &posMap, BlockMap &blockMap)
{
	auto positions = blockMap[bid];

	if (positions.size() == 0) {
		// The block has never been written to
		fputs("Attempting to read block that hasn't been written", stderr);
	
		// Pick a free position
		return GetFreePosition(posMap, blockMap);
	}

	// Pick random element to read
	size_t pos = positions[rand() % positions.size()];
	return pos;
}

block Agent::Access(Op op, int64_t bid, block data)
{
	// Load position map
	PositionMap posMap = LoadPositionMap();
	BlockMap blockMap = GenerateBlockMap(posMap);
	ChangedMap changedMap = LoadChangedMap();
	
	block ciphertext;
	block plaintext;

	if (op == READ) {
		size_t pos = GetBlockPosition(bid, posMap, blockMap);

		// Retrieve plaintext
		ciphertext = store->Read(pos);
 		plaintext = AES::Decrypt(key, ciphertext);
	} else {
		// TODO: Fake a read here
	}

	// Generate a new position
	size_t newPos = GetFreePosition(posMap, blockMap);
	
	// Re-encrypt the data
	if (op == WRITE) {
		ciphertext = AES::Encrypt(key, data);
	} else {
		ciphertext = AES::Encrypt(key, plaintext);
	}

	store->Write(newPos, ciphertext);
	
	if (op == WRITE) {
		// Remove old copies of data if updated
		for (auto pos : blockMap[bid]) {
			posMap[pos] = -1;
		}

		// Notify that the block has been change
		changedMap[bid] = 1;
	}

	posMap[newPos] = bid;
	
	// Don't need to save blockmap
	SavePositionMap(posMap);
	SaveChangedMap(changedMap);

	return plaintext;
}

void Agent::Duplicate(int64_t bid, block plaintext)
{
	PositionMap posMap = LoadPositionMap();
	BlockMap blockMap = GenerateBlockMap(posMap);

	// Find a free position
	size_t newPos = GetFreePosition(posMap, blockMap);
	
	block ciphertext = AES::Encrypt(key, plaintext);

	store->Write(newPos, ciphertext);

	posMap[newPos] = bid;

	SavePositionMap(posMap);
}

size_t Agent::GetBlockCount()
{
	return count;
}

size_t Agent::GetBlockSize()
{
	return size;
}
