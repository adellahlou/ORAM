#pragma once

#include "BlockStore.hpp"
#include "AES.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

using PositionMap = std::vector<int64_t>;
using BlockMap = std::unordered_map<int64_t, std::vector<size_t>>;

class Agent {
	BlockStore *store;
	
	size_t count, size;

	bytes<Key> key;
	
	block Encrypt(block plaintext);
	block Decrypt(block ciphertext);

public:
	Agent(BlockStore *store, size_t count, size_t size, bytes<Key> key);
	~Agent();

	enum Op {
		READ,
		WRITE
	};
	
	block Access(Op op, int64_t bid, block data);

	PositionMap LoadPositionMap();

	static BlockMap GenerateBlockMap(PositionMap posMap);

	size_t GetBlockCount();
	size_t GetBlockSize();
};
