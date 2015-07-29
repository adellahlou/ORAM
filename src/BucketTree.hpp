#pragma once

#include "Storage.hpp"

#include <fstream>
#include <cmath>

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
