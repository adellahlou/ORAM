#include "ORAM.hpp"

#include <cstdio>
#include <cstdlib>

#include <fstream>

int read(ORAM &oram)
{
	puts("reading");
	
	std::ifstream file;
	file.open("input.txt", std::fstream::in | std::fstream::binary);
	
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	
	for (int i = 0; i < length; i += CHUNK) {
		int readLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
	
		Chunk buffer;
		file.read(buffer.data(), readLength);
		oram.Write(buffer, blockID);
	
		printf("\r%d / %d", blockID, length/CHUNK);
		fflush(stdout);
	}
	
	file.close();
	puts("\n");
	
	return length;
}

void write(ORAM &oram, int length)
{
	puts("writing");
	
	std::fstream file;
	file.open("output.txt", std::fstream::out | std::fstream::binary | std::fstream::trunc);
		
	for (int i = 0; i < length; i += CHUNK) {
		int writeLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
		
		Chunk buffer;
		oram.Read(buffer, blockID);
		file.write(buffer.data(), writeLength);
		
		printf("\r%d / %d", i/CHUNK, length/CHUNK);
		fflush(stdout);
	}
	file.close();
	puts("\n");
}

int main()
{
	printf("total blocks = %d\n", BUCKETS*Z);

	ORAM oram;
	int length = read(oram);
	
	write(oram, length);
}
