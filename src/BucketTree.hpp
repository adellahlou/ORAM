#pragma once

#include <functional>
#include <cmath>

constexpr int DEPTH = 16;
constexpr int BUCKETS = ((int) pow(2, DEPTH+1) - 1);
constexpr int Z = 4;
constexpr int CHUNK = 1024*4;

/*
Each Blocks contain small amounts
of data and a unique identifier
*/
struct Block {
	int id;
	mutable char data[CHUNK];
	
	bool operator==(const Block &b) const {
		return id == b.id;
	}
};

struct BlockHash {
	std::hash<int> idHasher;

	size_t operator()(const Block &b) const {
		return idHasher(b.id);
	}
};

/*
Buckets contain multiple blocks
*/
struct Bucket{
	Block blocks[Z];
};

class BucketTree {
	//std::fstream file
	
	Bucket *tree;
	
public:
	BucketTree(std::string filename);
	~BucketTree();
	
	Bucket Read(int bucketPos);
	Bucket Write(Bucket newBucket, int bucketPos);
};	
