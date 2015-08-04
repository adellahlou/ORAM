#pragma once

#include "Stash.hpp"
#include "PositionMap.hpp"
#include "BucketTree.hpp"

#include <random>
#include <vector>
#include <unordered_map>

class ORAM {
	BucketTree tree;
	PositionMap position;
	Stash stash;
	
	// Randomness
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dis;
	
	int RandomPath();
	int GetNodeOnPath(int leaf, int depth);
	std::vector<int> GetIntersectingBlocks(int x, int depth);
	
	void FetchPath(int x);
	void WritePath(int x);
	
	void ReadData(Chunk &chunk, int blockID);
	void WriteData(Chunk &chunk, int blockID);
	
public:
	ORAM(int depth);
	~ORAM();
	
	enum Op {
		READ,
		WRITE
	};
	
	void Access(Op op, Chunk &chunk, int blockID);
	
	int GetDepth() const;
	int GetBlocks() const;
	int GetBuckets() const;
};
