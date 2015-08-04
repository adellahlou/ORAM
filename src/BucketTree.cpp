#include "BucketTree.hpp"
#include "File.hpp"

#include <functional>
#include <cstring>

#include <sys/mman.h>
#include <unistd.h>
#include <sys/fcntl.h>

raw<Key> key = {0};

BucketTree::BucketTree(std::string filename, int depth)
: filename(filename), depth(depth)
{
	fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);

	if (fd == -1) {
		perror("Error opening file");
		exit(1);
	}
	
	size_t fileSize = GetBuckets()*BucketCipher;
	
	// Retrieve the current size of the file
	size_t currentSize = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	
	// Resize file if it's not the right size
	if (currentSize != fileSize) {
		puts("TRUCATE!!!");
		ftruncate(fd, fileSize);
	}
	
	printf("current tree size = %zu\n", currentSize);
	
	map = (raw_t *) mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (map == MAP_FAILED) {
		perror("Error mapping file");
		close(fd);
		exit(1);
	}
	
	// Initialise the file if resized
	if (currentSize != fileSize) {
		puts("populating tree with dummy blocks");
		for (int i = 0; i < GetBuckets(); i++) {
			Bucket bucket;
	
			for (int z = 0; z < Z; z++) {
				Block &b = bucket[z];
				b.id = -1;
				
				if (1) {
					b.data = {0};
				} else {
					// random
				}
			}
	
			raw<BucketCipher> ciphertext;
		
			BucketHelper::Encrypt(key, bucket, ciphertext);
		
			memcpy(map + i*BucketCipher, ciphertext.data(), ciphertext.size());
		}
		puts("creating new tree file");
	} else {
		puts("using existing tree file");
	}
}

BucketTree::~BucketTree()
{
	if(munmap(map, GetBuckets()*BucketCipher) == -1) {
		perror("Failed to unmap file");
	}
	
	close(fd);
}

// Gets a bucket from the tree
void BucketTree::Read(Bucket &b, int pos)
{
	raw<BucketCipher> ciphertext;
	
	// Read in ciphertext from file
	memcpy(ciphertext.data(), map + pos*BucketCipher, ciphertext.size());
	
	BucketHelper::Decrypt(key, ciphertext, b);
}

// Writes a new bucket and returns
// the previous bucket value
void BucketTree::Write(Bucket &b, int pos)
{
	raw<BucketCipher> ciphertext;
	BucketHelper::Encrypt(key, b, ciphertext);

	// Retrieve old contents
	// Read(b, pos); (CATASTROPHIC BUG)

	// Write ciphertext to file
	memcpy(map + pos*BucketCipher, ciphertext.data(), ciphertext.size());
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
