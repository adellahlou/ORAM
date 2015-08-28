#pragma once

#include "BlockStore.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

using PositionMap = std::vector<int64_t>;
using BlockMap = std::unordered_map<int64_t, std::vector<size_t>>;

class Agent {
	std::shared_ptr<BlockStore> store;

	block Encrypt(block plaintext);
	block Decrypt(block ciphertext);

public:
	Agent(BlockStore *store);
	~Agent();

	enum Op {
		READ,
		WRITE
	};
	
	void Access(Op op, int64_t bid, block data);

	size_t GetNum();
};
