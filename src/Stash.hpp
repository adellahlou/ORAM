#pragma once

#include "Storage.hpp"

#include <unordered_map>

using Stash = std::unordered_map<int, Chunk>;

class StashHelper {
public:
	static bool Load(std::string filename, Stash &stash);
	static void Save(std::string filename, Stash &stash);
};
