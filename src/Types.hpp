#pragma once

#include <array>

// The main type for passing around
// raw file data
using byte_t = uint8_t;

template <long unsigned int N>
using bytes = std::array<byte_t, N>;

// The size of data in each Block
constexpr size_t CHUNK = 1024*4; // 4 KB
using Chunk = bytes<CHUNK>;

// Each Blocks contain a unique
// identifier and a chunk of data
struct Block {
	int32_t id;
	Chunk data;
};

// A bucket contains a number of Blocks
constexpr int Z = 4;
using Bucket = std::array<Block, Z>;
