#include "BucketTree.hpp"

BucketTree::BucketTree(std::string filename)
: filename(filename)
{
	file.open(filename, std::fstream::in | std::fstream::out | std::ios::binary);
	
	tree = new Bucket[BUCKETS];
	
	// Get the length of the file
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);
	
	if (length == BUCKETS*sizeof (Bucket)) {
		printf("reading tree file\n");
		file.read((char *) tree, BUCKETS*sizeof (Bucket));
		return;
	}
	
	// Otherwise initialise new bucketS	
	for (int i = 0; i < BUCKETS; i++) {
		for (int z = 0; z < Z; z++) {
			tree[i][z].id = -1;
		}
	}
	
	file.close();
}

BucketTree::~BucketTree()
{
	file.open(filename, std::fstream::out | std::ios::binary | std::ios::trunc);
	file.seekg(0, std::ios::beg);
	file.write((char *) tree, BUCKETS*sizeof (Bucket));
	
	file.close();
	
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
