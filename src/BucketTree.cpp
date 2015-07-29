#include "BucketTree.hpp"
#include "File.hpp"

#include <functional>

BucketTree::BucketTree(std::string filename, int depth)
: filename(filename), depth(depth)
{
	tree = new Bucket[GetBuckets()];
	
	/*
	file.open(filename, std::fstream::in | std::fstream::out | std::ios::binary);	
	
	// Get the length of the file
	size_t length = File::GetLength(file);
	
	if (length == GetBuckets()*sizeof (Bucket)) {
		printf("reading tree file\n");
		file.read((char *) tree, GetBuckets()*sizeof (Bucket));
		return;
	}

	file.close();
	*/
	
	// Otherwise initialise new blocks
	for (int i = 0; i < GetBuckets(); i++) {
		for (int z = 0; z < Z; z++) {
			tree[i][z].id = -1;
		}
	}
	
}

BucketTree::~BucketTree()
{
	/*
	// Dump tree contents to file
	file.open(filename, std::fstream::out | std::ios::binary | std::ios::trunc);
	file.seekg(0, std::ios::beg);
	file.write((char *) tree, GetBuckets()*sizeof (Bucket));
	
	file.close();
	*/
	
	delete[] tree;
}

// Gets a bucket from the tree
void BucketTree::Read(Bucket &b, int pos)
{
	b = tree[pos];
}

// Writes a new bucket and returns
// the previous bucket value
void BucketTree::Write(Bucket &b, int pos)
{
	std::swap(b, tree[pos]);
}

int BucketTree::GetDepth() const
{
	return depth;
}

int BucketTree::GetBlocks() const
{
	return GetBuckets()*Z;
}

int BucketTree::GetBuckets() const
{
	int bucketCount = pow(2, GetDepth() + 1) - 1;
	return bucketCount;
}
