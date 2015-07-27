#pragma once

#include "BucketTree.hpp"

#include <vector>
#include <random>
#include <unordered_set>


class ORAM {
	int *position;
	//struct Bucket *tree;
	BucketTree tree;
	
	std::unordered_set<Block, BlockHash> stash;
	
	// Randomness
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dis;
	
	int RandomPath();
	
	void FetchPath(int x);
	void WritePath(int x);
	
	void ReadData(char *data, int blockID);
	void WriteData(char *data, int blockID);
	
public:
	ORAM();
	~ORAM();
	
	void Read(char *data, int blockID);
	void Write(char *data, int blockID);
};
