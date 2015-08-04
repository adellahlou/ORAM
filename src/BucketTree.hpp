#pragma once

#include "Storage.hpp"
#include "Bucket.hpp"

#include <cmath>

class BucketTree {
	std::string filename;
	int fd;
	raw_t *map;
	
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
