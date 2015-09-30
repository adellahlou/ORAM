#pragma once

#include <cstddef>
#include <unordered_map>
#include <list>
#include <vector>

struct FileInfo {
	size_t length;
	std::vector<size_t> blocks;
};

class BlockStore;

class FileSystem {
	BlockStore *store;
	
	std::unordered_map<std::string, FileInfo> files;
	
	int GetAvailableID();

	std::pair<std::string, FileInfo> LoadFileInfo(std::string line);
	std::string SaveFileInfo(std::string filename, FileInfo info);

public:
	FileSystem(BlockStore *store);
	~FileSystem();

	bool Add(std::string filename);
	bool Remove(std::string filename);
	
	void Load();
	void Save();
	
	BlockStore *GetBlockStore();

	FileInfo GetFileInfo(std::string filename);
};
