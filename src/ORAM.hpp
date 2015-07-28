#pragma once

#include "BucketTree.hpp"

#include <vector>
#include <random>
#include <unordered_set>
#include <unordered_map>

class ORAM {
	int *position;
	BucketTree tree;
	
	//std::unordered_set<Block, BlockHash> stash;
	std::unordered_map<int, Chunk> stash;
	
	// Randomness
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dis;
	
	int RandomPath();
	
	void FetchPath(int x);
	void WritePath(int x);
	
	void ReadData(Chunk &chunk, int blockID);
	void WriteData(Chunk &chunk, int blockID);
	
public:
	ORAM();
	~ORAM();
	
	void Read(Chunk &chunk, int blockID);
	void Write(Chunk &chunk, int blockID);
};
