#include "FileStorage.hpp"
#include "ORAM.hpp"
#include "File.hpp"

FileStorage::FileStorage(ORAM &oram)
: oram(oram), files(), availableIDs(oram.GetBlocks())
{
	for (int i = 0; i < oram.GetBlocks(); i++) {
		availableIDs.push_back(i);
	}
}

FileStorage::~FileStorage()
{}

int FileStorage::GetAvailableID()
{
	int index = rand() % availableIDs.size();
	int id = availableIDs[index];
	
	// ID is no longer available
	availableIDs.erase(availableIDs.begin() + index);
	
	return id;
}

void FileStorage::FreeID(int id)
{
	availableIDs.push_back(id);
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
	
	FileInfo info;
	info.length = File::GetLength(file);
	
	for (size_t i = 0; i < info.length; i += CHUNK) {
		int readLength = std::min(CHUNK, info.length - i);
		
		Chunk chunk = {0};
		File::Read(file, chunk.data(), readLength);
		
		// Generate random blockID
		int id = GetAvailableID();
		info.blocks.push_back(id);
		
		oram.Access(ORAM::WRITE, chunk, id);
	}
	
	file.close();
	
	// Keep track of file info
	files[filename] = info;
	
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
