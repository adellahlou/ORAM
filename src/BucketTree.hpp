#pragma once

#include <array>
#include <fstream>
#include <functional>

#include <cmath>

constexpr size_t CHUNK = 1024*4; // 4 KB
using Chunk = std::array<char, CHUNK>;

/*
Each Blocks contain a unique
identifier and a chunk of data
*/
struct Block {
	int32_t id;
	Chunk data;
};

constexpr int Z = 4;
using Bucket = std::array<Block, Z>;

/*
struct Bucket {
	uint8_t iv[16];
	std::vector<Block> blocks;
};
*/

class BucketTree {
	std::string filename;
	std::fstream file;
	
	Bucket *tree;
	const int depth;
	
public:
	BucketTree(std::string filename, int depth);
	~BucketTree();
	
	void Read(Bucket &b, int pos);
	void Write(Bucket &b, int pos);
	
	int GetDepth() const;
	int GetBlocks() const;
	int GetBuckets() const;
};	
