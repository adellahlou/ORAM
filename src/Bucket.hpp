#pragma once

#include "BlockStore.hpp"
#include "AES.hpp"

// The size of blocks
constexpr size_t ChunkSize = 1024*4; // 4 KiB

using Chunk = bytes<ChunkSize>;

// Each Blocks contain a unique
// identifier and a chunk of data
struct Block {
	int32_t id;
	Chunk data;
};

// A bucket contains a number of Blocks
constexpr int Z = 4;
using Bucket = std::array<Block, Z>;

constexpr size_t BucketCipher = IV + (sizeof (Bucket)/16 + 1)*16;

class BucketHelper {
public:
	static int Encrypt(bytes<Key> key, Bucket &b, bytes<BucketCipher> &ciphertext);
	static int Decrypt(bytes<Key> key, bytes<BucketCipher> &ciphertext, Bucket &b);
};
