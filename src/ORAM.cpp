#include "ORAM.hpp"

#include <unordered_set>
#include <algorithm>
#include <random>

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cmath>

ORAM::ORAM()
: tree("tree.txt"), rd(), mt(rd()), dis(0, BUCKETS/2)
{
	// Maps Block IDs to paths
	position = new int[BUCKETS*Z];
	
	// Initialise blocks with random paths
	for (int i = 0; i < BUCKETS*Z; i++) {
		position[i] = RandomPath();
	}
}

ORAM::~ORAM()
{
	delete[] position;
}

// Generate a random path (a leaf node)
int ORAM::RandomPath()
{
	return dis(mt);
}

// Fetches the array index a bucket
// that lise on a specific path
int GetNodeOnPath(int leaf, int depth)
{
	leaf += BUCKETS/2;
	for (int d = DEPTH - 1; d >= depth; d--) {
		leaf = (leaf + 1)/2 - 1;
	}
	
	return leaf;
}

// Fetches blocks along a path, adding
// them to the stash
void ORAM::FetchPath(int x)
{
	for (int d = 0; d <= DEPTH; d++) {
		Bucket bucket;
		tree.Read(bucket, GetNodeOnPath(x, d));
		
		for (int z = 0; z < Z; z++) {
			Block &block = bucket[z];
		
			if (block.id == -1) {
				continue;
			}
			
			stash.insert({block.id, block.data});
		}
	}
}

// Greedily writes blocks from the stash to the tree
void ORAM::WritePath(int x)
{
	std::vector<int> validBlocks;
	
	// Write back the path
	for (int d = DEPTH; d >= 0; d--) {
		validBlocks.clear();
		int node = GetNodeOnPath(x, d);
		
		// Find blocks that can be on this bucket
		for (auto b : stash) {
			if (GetNodeOnPath(position[b.first], d) == node) {
				validBlocks.push_back(b.first);
			}
		}
		
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
