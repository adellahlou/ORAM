#include "BucketTree.hpp"

BucketTree::BucketTree(std::string filename)
{
	//file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
	tree = new Bucket[BUCKETS];
	
	for (int i = 0; i < BUCKETS; i++) {
		for (int z = 0; z < Z; z++) {
			tree[i].blocks[z].id = -1;
		}
	}
}

BucketTree::~BucketTree()
{
	//file.close();
	delete[] tree;
}

// Gets the block from the tree
Bucket BucketTree::Read(int bucketPos)
{
	//file.seek(sizeof (Block) * block);
	
	return tree[bucketPos];
}

// Writes a new block, and returns the
// old blocks
Bucket BucketTree::Write(Bucket newBucket, int bucketPos)
{
	Bucket oldBucket = tree[bucketPos];
	tree[bucketPos] = newBucket;
	
	return oldBucket;
}
