#include "FileStore.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

static size_t GetFileSize(int fd)
{
	// Jump to the end of the file
	size_t size = lseek(fd, 0L, SEEK_END);
	
	// Return back to the start
	lseek(fd, 0L, SEEK_SET);
	
	return size;
}

FileStore::FileStore(std::string filename, size_t size)
: size(size)
{
	// Open the file
	int oflags = O_RDWR | O_CREAT;
	fd = open(filename.c_str(), oflags, 0666);

	if (fd == -1) {
		perror("Error opening file");
		exit(1);
	}

	size_t currentSize = GetFileSize(fd);
	size_t totalSize = size*BlockSize;

	// Resize the file if needed
	if (currentSize != totalSize) {
		ftruncate(fd, totalSize);
	}

	// Map the file to memory
	int mflags = PROT_READ | PROT_WRITE;
	map = (byte_t *) mmap(nullptr, totalSize, mflags, MAP_SHARED, fd, 0);

	if (map == MAP_FAILED) {
		perror("Error mapping file");
		close(fd);
		exit(1);
	}
}

FileStore::~FileStore()
{
	if (munmap(map, size*BlockSize) == -1) {
		perror("Failed to unmap file");
		exit(1);
	}

	close(fd);
}

block FileStore::Read(size_t pos)
{
	block b;
	//memcpy(b.data(), &map[pos*BlockSize], BlockSize);

	std::copy(&map[pos*BlockSize], &map[pos*BlockSize] + BlockSize, b.begin());

	return b;
}

void FileStore::Write(size_t pos, block b)
{
	//memcpy(&map[pos*BlockSize], b.data(), BlockSize);
	std::copy(b.begin(), b.end(), &map[pos*BlockSize]);
}

size_t FileStore::GetSize()
{
	return size;
}
