#include "FileStorage.hpp"
#include "ORAM.hpp"
#include "File.hpp"

#include <sstream>
#include <algorithm>

FileStorage::FileStorage(ORAM &oram)
: oram(oram), files()//, availableIDs(oram.GetBlocks())
{
	for (int i = 0; i < oram.GetBlocks(); i++) {
		
		//availableIDs.push_back(i);
		//availableIDs[i] = i;
	}
}

FileStorage::~FileStorage()
{}

int FileStorage::GetAvailableID()
{
	// Randomise the ID order
	//std::random_shuffle(availableIDs.begin(), availableIDs.end());

	/*
	int index = rand() % availableIDs.size();
	int id = availableIDs[index];
	
	availableIDs[index] = availableIDs.back();
	availableIDs.pop_back();
	
	return id;
	*/
	
	/*
	for (;;) {
		int id = rand() % oram.GetBlocks();
		
		bool found = false;
		
		// Check if the ID is in use
		for (auto f : files) {
			auto &blocks = f.second.blocks;
			auto iter = std::find(blocks.begin(), blocks.end(), id);
			
			if (iter != blocks.end()) {
				found = true;
				break;
			}
		}
		
		if (found) {
			continue;
		}
		
		// This ID is free
		return id;
	}*/
	
	// We don't delete files (yet)
	static int id = 0;
	return id++;
}

void FileStorage::FreeID(int id)
{
	//availableIDs.push_back(id);
}

bool FileStorage::Add(std::string filename)
{
	if (files.find(filename) != files.end()) {
		puts("error: file already exists");
		return false;
	}
	
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::binary);
	
	if (!file) {
		puts("failed to open file");
		return false;
	}
	
	// Keep track of file info
	FileInfo &info = files[filename];
	info.length = File::GetLength(file);
	
	for (size_t i = 0; i < info.length; i += ChunkSize) {
		int readLength = std::min(ChunkSize, info.length - i);
		
		Chunk chunk = {0};
		File::Read(file, chunk.data(), readLength);
		
		// Generate random blockID
		int bid = GetAvailableID();
		info.blocks.push_back(bid);
		
		oram.Access(ORAM::WRITE, bid, chunk);
		
		//printf("\r%zu / %zu", i/CHUNK + 1, info.length/CHUNK);
		//fflush(stdout);
	}
	//puts("");
	
	file.close();
	
	return true;
}

bool FileStorage::Remove(std::string filename)
{
	if (files.find(filename) == files.end()) {
		puts("error: file not in database");
		return false;
	}
	
	FileInfo &info = files[filename];
	
	for (int i : info.blocks) {
		FreeID(i);
	}
	
	files.erase(filename);
	
	return true;
}

std::pair<std::string, FileInfo> LoadFile(std::istringstream &sstream)
{
	std::string filename;
	sstream >> filename;
	
	FileInfo info;
	sstream >> info.length;

	for (size_t i = 0; i < info.length; i += ChunkSize) {
		int blockID;
		sstream >> blockID;
	
		info.blocks.push_back(blockID);
	}
	
	return std::make_pair(filename, info);
}

void FileStorage::Load()
{
	std::ifstream file("filemap.txt");
	
	if (!file) {
		return;
	}
	
	std::string line;
	
	while (std::getline(file, line)) {
		std::istringstream sstream(line);
	
		files.insert(LoadFile(sstream));
	}
	
	file.close();
}

std::string SaveFile(std::string filename, FileInfo info)
{
	std::ostringstream sstream;
	
	sstream << filename;
	sstream << ' ';
	sstream << info.length;
	
	for (size_t i = 0; i < info.length; i += ChunkSize) {
		sstream << ' ';
		sstream << info.blocks[i/ChunkSize];
	}
	
	return sstream.str();
}

void FileStorage::Save()
{
	std::ofstream file("filemap.txt");
	
	for (auto f : files) {
		file << SaveFile(f.first, f.second);
		file << '\n';
	}
	
	file.close();
}


FileInfo FileStorage::GetFileInfo(std::string filename)
{
	return files[filename];
}
