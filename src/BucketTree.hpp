#pragma once

#include "Types.hpp"
#include "Bucket.hpp"

#include <cmath>

class BucketTree {
	std::string filename;
	int fd;
	byte_t *map;
	
	const int depth;
	
public:
	BucketTree(std::string filename, int depth, bytes<Key> key);
	~BucketTree();
	
	void Read(Bucket &b, int pos, bytes<Key> key);
	void Write(Bucket &b, int pos, bytes<Key> key);
	
	int GetDepth() const;
	int GetBlocks() const;
	int GetBuckets() const;
};	
