#pragma once

#include "BlockStore.hpp"
#include "Stash.hpp"
#include "AES.hpp"

#include <random>
#include <vector>
#include <unordered_map>

// Each Blocks contain a unique
// identifier and a chunk of data
struct Block {
	int32_t id;
	block data;
};

// A bucket contains a number of Blocks
constexpr int Z = 4;
using Bucket = std::array<Block, Z>;

class ORAM: public BlockStore {
	BlockStore *store;
	
	size_t depth;	
	size_t blockSize;
	
	bytes<Key> key;
	
	int *position;
	Stash stash;
	
	bool wasSerialised;

	// Randomness
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dis;
	
	int RandomPath();
	int GetNodeOnPath(int leaf, int depth);
	std::vector<int> GetIntersectingBlocks(int x, int depth);
	
	void FetchPath(int x);
	void WritePath(int x);
	
	block ReadData(int bid);
	void WriteData(int bid, block b);
	
	block SerialiseBucket(Bucket bucket);
	Bucket DeserialiseBucket(block buffer);

	Bucket ReadBucket(int bid);
	void WriteBucket(int bid, Bucket bucket);

public:
	ORAM(BlockStore *store, size_t depth, size_t blockSize, bytes<Key> key);
	~ORAM();
	
	enum Op {
		READ,
		WRITE
	};
	
	void Access(Op op, int bid, block &b);

	block Read(size_t bid);
	void Write(size_t bid, block b);
	
	size_t GetDepth();

	size_t GetBlockCount();
	size_t GetBlockSize();

	size_t GetBucketCount();

	bool WasSerialised();
};
