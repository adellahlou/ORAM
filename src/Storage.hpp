#pragma once

#include <array>

using raw_t = uint8_t;

template <long unsigned int N>
using raw = std::array<raw_t, N>;

constexpr size_t CHUNK = 1024*4; // 4 KB
using Chunk = raw<CHUNK>;

/*
Each Blocks contain a unique
identifier and a chunk of data
*/
struct Block {
	int32_t id;
	Chunk data;
};

constexpr int Z = 4;
using Bucket = std::array<Block, Z>;

// Encryption constants
constexpr int IV = 16; // bytes
constexpr int Key = 32; // bytes
