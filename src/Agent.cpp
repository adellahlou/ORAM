#include "Agent.hpp"
#include "AES.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cinttypes>

Agent::Agent(BlockStore *store)
: store(store)
{}

Agent::~Agent()
{}

block Agent::Encrypt(block plaintext)
{
	bytes<IV> iv = AES::GenerateIV();
	bytes<Key> key;
	std::fill(key.begin(), key.end(), 0);

	block ciphertext = AES::Encrypt(key, iv, plaintext);

	// Put randomised IV at the front
	ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

	return ciphertext;
}

block Agent::Decrypt(block ciphertext)
{
	// Extract the IV
	bytes<IV> iv;
	std::copy(ciphertext.begin(), ciphertext.begin() + IV, iv.begin());

	ciphertext.erase(ciphertext.begin(), ciphertext.begin() + IV);

	bytes<Key> key;
	std::fill(key.begin(), key.end(), 0);

	// Perform the decryption
	block plaintext = AES::Decrypt(key, iv, ciphertext);

	return plaintext;
}

namespace {
	// TODO: Serialise the position map
	PositionMap *pm;

	PositionMap LoadPositionMap(size_t num)
	{
		if (!pm) {
			pm = new PositionMap(num);
	
			// All spaces are originally free
			for (size_t i = 0; i < pm->size(); i++) {
				pm->operator[](i) = -1;
			}
		}
		return *pm;
	}

	void SavePositionMap(PositionMap posMap)
	{
		*pm = posMap;
	}

	BlockMap GenerateBlockMap(PositionMap posMap)
	{
		BlockMap blockMap;

		for (size_t i = 0; i < posMap.size(); i++) {
			int64_t bid = posMap[i];
			blockMap[bid].push_back(i);
		}

		return blockMap;
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
}

void Agent::Access(Op op, int64_t bid, block data)
{
	// Load position map
	PositionMap posMap = LoadPositionMap(GetNum());
	BlockMap blockMap = GenerateBlockMap(posMap);

	block ciphertext;
	block plaintext;
	
	if (op == READ) {
		size_t pos = GetBlockPosition(bid, posMap, blockMap);

		// Retrieve ciphertext
		ciphertext = store->Read(pos);
		
 		plaintext = Decrypt(ciphertext);
	}

	// Generate a new position
	size_t newPos = GetFreePosition(posMap, blockMap);

	// Re-encrypt the data
	if (op == WRITE) {
		ciphertext = Encrypt(data);
	} else {
		ciphertext = Encrypt(plaintext);
	}

	store->Write(newPos, ciphertext);

	posMap[newPos] = bid;
	
	// dont need to save blockmap
	SavePositionMap(posMap);
}

size_t Agent::GetNum()
{
	return store->GetNum();
}
