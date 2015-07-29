#include "ORAM.hpp"

#include <unordered_set>
#include <algorithm>
#include <random>

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cmath>

ORAM::ORAM(int depth)
: tree("tree.bin", depth), position("pos.bin", tree.GetBlocks()), rd(), mt(rd()), dis(0, tree.GetBuckets()/2)
{
	if (!position.AlreadyExisted()) {
		// Initialise blocks with random paths
		for (int i = 0; i < tree.GetBlocks(); i++) {
			position[i] = RandomPath();
		}
	}
}

ORAM::~ORAM()
{}

// Generate a random path (a leaf node)
int ORAM::RandomPath()
{
	return dis(mt);
}

// Fetches the array index a bucket
// that lise on a specific path
int ORAM::GetNodeOnPath(int leaf, int depth)
{
	leaf += GetBuckets()/2;
	for (int d = GetDepth() - 1; d >= depth; d--) {
		leaf = (leaf + 1)/2 - 1;
	}
	
	return leaf;
}

// Fetches blocks along a path, adding
// them to the stash
void ORAM::FetchPath(int x)
{
	for (int d = 0; d <= GetDepth(); d++) {
		Bucket bucket;
		tree.Read(bucket, GetNodeOnPath(x, d));
		
		for (int z = 0; z < Z; z++) {
			Block &block = bucket[z];
		
			if (block.id != -1) {
				stash.insert({block.id, block.data});	
			}
		}
	}
}

std::vector<int> ORAM::GetIntersectingBlocks(int x, int depth)
{
	std::vector<int> validBlocks;
	
	int node = GetNodeOnPath(x, depth);
	
	for (auto b : stash) {
		int id = b.first;
		if (GetNodeOnPath(position[id], depth) == node) {
			validBlocks.push_back(id);
		}
	}
	
	return validBlocks;
}

// Greedily writes blocks from the stash to the tree
void ORAM::WritePath(int x)
{	
	// Write back the path
	for (int d = GetDepth(); d >= 0; d--) {
		// Find blocks that can be on this bucket
		int node = GetNodeOnPath(x, d);
		auto validBlocks = GetIntersectingBlocks(x, d);
		
		// Write blocks to tree
		Bucket bucket;
		for (int z = 0; z < std::min((int) validBlocks.size(), Z); z++) {
			Block &block = bucket[z];
			block.id = validBlocks[z];
			block.data = stash[block.id];
			// swap(position[b.blocks[z].id, blocks[z].id)
			
			// The block no longer needs to be in the stash
			stash.erase(block.id);
		}
		
		// Fill any empty spaces with dummy blocks
		for (int z = validBlocks.size(); z < Z; z++) {
			Block &block = bucket[z];
			block.id = -1;
		}
		
		// Write bucket to tree
		tree.Write(bucket, node);
	}
}

//Gets the data of a block in the stash
void ORAM::ReadData(Chunk &chunk, int blockID)
{
	// Create block if it doesn't exist
	auto iter = stash.find(blockID);
	if (iter == stash.end()) {
		stash[blockID].fill(0);
	}
	
	chunk = stash[blockID];
}

// Gets the data of a block in the stash and
// updates it with new data
void ORAM::WriteData(Chunk &chunk, int blockID)
{
	// Create block if it doesn't exist
	auto iter = stash.find(blockID);
	if (iter == stash.end()) {	
		stash[blockID].fill(0);
	}
	
	// Give back the previous data in the block
	std::swap(chunk, stash[blockID]);
}

// Reads data from a block
void ORAM::Read(Chunk &chunk, int blockID)
{
	int x = position[blockID];
	position[blockID] = RandomPath();
	
	FetchPath(x);
	ReadData(chunk, blockID);
	WritePath(x);
}

// Writes data from a block, replacing the buffer
// with the old data
void ORAM::Write(Chunk &chunk, int blockID)
{
	int x = position[blockID];
	position[blockID] = RandomPath();
	
	FetchPath(x);
	WriteData(chunk, blockID);	
	WritePath(x);
}

int ORAM::GetDepth() const
{
	return tree.GetDepth();
}

int ORAM::GetBlocks() const
{
	return tree.GetBlocks();
}

int ORAM::GetBuckets() const
{
	return tree.GetBuckets();
}
	
