#include "ORAM.hpp"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <unordered_set>
#include <algorithm>
#include <random>

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

// Fetches the node on a specific path
int GetNodeOnPath(int leaf, int depth)
{
	leaf += BUCKETS/2;
	for (int d = DEPTH-1; d >= depth; d--) {
		leaf = (leaf+1)/2-1;
	}
	
	return leaf;
}

// Fetches blocks along a path to the stash
void ORAM::FetchPath(int x)
{
	for (int d = 0; d <= DEPTH; d++) {
		Bucket b = tree.Read(GetNodeOnPath(x, d));
		
		for (int z = 0; z < Z; z++) {
			if (b.blocks[z].id != -1) {
				stash.insert(b.blocks[z]);
			}
		}
	}
}


// Greedily writes blocks from the stash to the tree
void ORAM::WritePath(int x)
{
	std::vector<Block> blocks;
	
	for (int d = DEPTH; d >= 0; d--) {
		int node = GetNodeOnPath(x, d);
		blocks.clear();
		
		// Find blocks that can be on this path
		for (auto b : stash) {
			if (GetNodeOnPath(position[b.id], d) == node) {
				blocks.push_back(b);
			}
		}
		
		// Write blocks to tree
		Bucket b;
		for (int z = 0; z < std::min((int) blocks.size(), Z); z++) {
			b.blocks[z] = blocks[z];
			stash.erase(blocks[z]);
		}
		
		// Fill any empty spaces with dummy blocks
		for (int z = blocks.size(); z < Z; z++) {
			b.blocks[z].id = -1;
		}
		
		tree.Write(b, node);
	}
}

//Gets the data of a block in the stash
void ORAM::ReadData(char *data, int blockID)
{
	auto iter = stash.find({blockID});
	
	// Create block if it doesn't exist
	if (iter == stash.end()) {
		Block b;
		b.id = blockID;
		
		// New blocks are zeroed out
		memset(b.data, 0, CHUNK);
		stash.insert(b);
		
		iter = stash.find({blockID});
		//assert(iter != stash.end())
	}
	
	memcpy(data, iter->data, CHUNK);
}

// Gets the data of a block in the stash and
// updates it with new data
void ORAM::WriteData(char *data, int blockID)
{
	// Get the target block's data
	auto iter = stash.find({blockID});
	
	// Create block if it doesn't exist
	if (iter == stash.end()) {
		Block b;
		b.id = blockID;
		memcpy(b.data, data, CHUNK);
		memset(data, 0, CHUNK);
		stash.insert(b);
	} else {
		// Exchange new and old data
		char buffer[CHUNK];
		
		memcpy(buffer, data, CHUNK);
		memcpy(data, iter->data, CHUNK);
		memcpy(iter->data, buffer, CHUNK);
	}
}

// Reads data from a block
void ORAM::Read(char *data, int blockID)
{
	int x = position[blockID];
	position[blockID] = RandomPath();
	
	FetchPath(x);
	ReadData(data, blockID);
	WritePath(x);
}

// Writes data from a block, giving the old data
void ORAM::Write(char *data, int blockID)
{
	int x = position[blockID];
	position[blockID] = RandomPath();
	
	FetchPath(x);
	WriteData(data, blockID);	
	WritePath(x);
}
