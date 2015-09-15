#pragma once

#include "BlockStore.hpp"
#include "AES.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

using PositionMap = std::vector<int64_t>;
using BlockMap = std::unordered_map<int64_t, std::vector<size_t>>;
using ChangedMap = std::vector<byte_t>;

class Agent {
	BlockStore *store;
	
	size_t count, size;

	bytes<Key> key;

public:
	Agent(BlockStore *store, size_t count, size_t size, bytes<Key> key);
	~Agent();

	enum Op {
		READ,
		WRITE
	};
	

	PositionMap LoadPositionMap();

	ChangedMap LoadChangedMap();
	void SaveChangedMap(ChangedMap changedMap);

	static BlockMap GenerateBlockMap(PositionMap posMap);

	block Access(Op op, int64_t bid, block data);
	
	void Duplicate(int64_t bid, block plaintext);

	size_t GetBlockCount();
	size_t GetBlockSize();
};
