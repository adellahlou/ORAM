#pragma once

#include "Bucket.hpp"

#include <unordered_map>

using Stash = std::unordered_map<int, Chunk>;

struct StashHelper {
	static bool Load(std::string filename, Stash &stash);
	static void Save(std::string filename, Stash &stash);
};
