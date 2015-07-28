#pragma once

#include <array>
#include <fstream>
#include <functional>

#include <cmath>

constexpr int DEPTH = 7;
constexpr int BUCKETS = ((int) pow(2, DEPTH+1) - 1);
constexpr int Z = 4;
constexpr int CHUNK = 1024*4;

using Chunk = std::array<char, CHUNK>;

/*
Each Blocks contain small amounts
of data and a unique identifier
*/
struct Block {
	int32_t id;
	Chunk data;
};

/*
Buckets just contain Z blocks.
*/
using Bucket = std::array<Block, Z>;

struct BlockHash {
	std::hash<int> idHasher;

	size_t operator()(const Block &b) const {
		return idHasher(b.id);
	}
};

class BucketTree {
	std::string filename;
	std::fstream file;
	
	Bucket *tree;
	
public:
	BucketTree(std::string filename);
	~BucketTree();
	
	void Read(Bucket &b, int pos);
	void Write(Bucket &b, int pos);
};	
