#include "FileSystem.hpp"
#include "BlockStore.hpp"
#include "File.hpp"
#include "Log.hpp"

#include <sstream>
#include <algorithm>

FileSystem::FileSystem(BlockStore *store)
: store(store), files()
{}

FileSystem::~FileSystem()
{}

int FileSystem::GetAvailableID()
{
	static size_t id = 0;
	
	if (id + 1 >= store->GetBlockCount()) {
		Log::Write(Log::FATAL, "out of space");
	}
	
	return id++;
}

bool FileSystem::Add(std::string filename)
{
	if (files.find(filename) != files.end()) {
		Log::Write(Log::WARNING, "file already exists");
		return false;
	}
	
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::binary);
	
	if (!file) {
		Log::Write(Log::WARNING, "failed to open file");
		return false;
	}
	
	// Keep track of file info
	FileInfo &info = files[filename];
	info.length = File::GetLength(file);
	
	size_t blockSize = store->GetBlockSize();
		
	for (size_t i = 0; i < info.length; i += blockSize) {
		size_t readLength = std::min(blockSize, info.length - i);
		
		block b(blockSize, 0);
		File::Read(file, b.data(), readLength);
		
		// Generate random blockID
		size_t bid = GetAvailableID();
		info.blocks.push_back(bid);
	
		store->Write(bid, b);	
		
		//printf("\r%zu / %zu", i/CHUNK + 1, info.length/CHUNK);
		//fflush(stdout);
	}
	//puts("");
	
	file.close();
	
	return true;
}

bool FileSystem::Remove(std::string filename)
{
	if (files.find(filename) == files.end()) {
		Log::Write(Log::WARNING, "file not in database");
		return false;
	}
	
	// TODO: This doesn't actually free the IDs

	files.erase(filename);
	
	return true;
}

std::pair<std::string, FileInfo> FileSystem::LoadFileInfo(std::string line)
{		
	std::istringstream sstream(line);

	std::string filename;
	sstream >> filename;
	
	FileInfo info;
	sstream >> info.length;

	size_t blockSize = store->GetBlockSize();

	for (size_t i = 0; i < info.length; i += blockSize) {
		size_t bid;
		sstream >> bid;
	
		info.blocks.push_back(bid);
	}
	
	return std::make_pair(filename, info);
}

void FileSystem::Load()
{
	std::ifstream file("filemap.txt");
	
	if (!file) {
		return;
	}
	
	std::string line;

	while (std::getline(file, line)) {
	
		files.insert(LoadFileInfo(line));
	}
	
	file.close();
}

std::string FileSystem::SaveFileInfo(std::string filename, FileInfo info)
{
	std::ostringstream sstream;
	
	sstream << filename;
	sstream << ' ';
	sstream << info.length;

	size_t blockSize = store->GetBlockSize();
	
	for (size_t i = 0; i < info.length; i += blockSize) {
		sstream << ' ';
		sstream << info.blocks[i/blockSize];
	}
	
	return sstream.str();
}

void FileSystem::Save()
{
	std::ofstream file("filemap.txt");
	
	for (auto f : files) {
		file << SaveFileInfo(f.first, f.second);
		file << '\n';
	}
	
	file.close();
}

BlockStore *FileSystem::GetBlockStore()
{
	return store;
}

FileInfo FileSystem::GetFileInfo(std::string filename)
{
	return files[filename];
}
