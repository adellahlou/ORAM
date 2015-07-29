#pragma once

#include "BucketTree.hpp"

#include <vector>
#include <random>
#include <unordered_map>

class ORAM {
	int *position;
	BucketTree tree;
	
	std::unordered_map<int, Chunk> stash;
	
	// Randomness
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dis;
	
	int RandomPath();
	int GetNodeOnPath(int leaf, int depth);
	
	void FetchPath(int x);
	void WritePath(int x);
	
	void ReadData(Chunk &chunk, int blockID);
	void WriteData(Chunk &chunk, int blockID);
	
public:
	ORAM(int depth);
	~ORAM();
	
	void Read(Chunk &chunk, int blockID);
	void Write(Chunk &chunk, int blockID);
	
	int GetDepth() const;
	int GetBlocks() const;
	int GetBuckets() const;
};
