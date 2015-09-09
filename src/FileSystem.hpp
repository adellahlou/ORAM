#pragma once

#include <cstddef>
#include <unordered_map>
#include <list>
#include <vector>

struct FileInfo {
	size_t length;
	std::vector<int> blocks;
};

class ORAM;

class FileStorage {
	ORAM &oram;
	
	std::unordered_map<std::string, FileInfo> files;
	
	//std::vector<int> availableIDs;

	int GetAvailableID();
	void FreeID(int id);
	
public:
	FileStorage(ORAM &oram);
	~FileStorage();

	bool Add(std::string filename);
	bool Remove(std::string filename);
	
	void Load();
	void Save();
	
	FileInfo GetFileInfo(std::string filename);
};
