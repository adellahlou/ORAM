#pragma once

#include "Types.hpp"

#include <vector>

// The size of blocks
constexpr size_t BlockSize = 1024*4; // 4 KiB

using block = bytes<BlockSize>;

/*
 * An abstract class for interfacing with
 * various different block devices
 */
class BlockStore {
public:
	virtual ~BlockStore() = default;

	virtual block Read(size_t pos) = 0;
	virtual void Write(size_t pos, block b) = 0;

	virtual size_t GetSize() = 0;
};
