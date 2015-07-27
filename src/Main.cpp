#include "ORAM.hpp"

#include <cstdio>
#include <cstdlib>

#include <fstream>

int main()
{
	printf("total blocks = %d\n", BUCKETS*Z);
	std::fstream file;
	file.open("input.txt", std::fstream::in | std::fstream::binary);

	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	
	ORAM oram;
	
	printf("reading\n");
	
	for (int i = 0; i < length; i += CHUNK) {
		int readLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
	
		char buffer[CHUNK] = {0};
		file.read(buffer, readLength);
		oram.Write(buffer, blockID);
	
		printf("\r%d / %d", blockID, length/CHUNK);
		fflush(stdout);
	}
	
	file.close();
	
	// play around with blocks
	
	file.open("output.txt", std::fstream::out | std::fstream::binary | std::fstream::trunc);
	printf("\n\nwriting\n");
	
	for (int i = 0; i < length; i += CHUNK) {
		int writeLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
		
		char buffer[CHUNK] = {0};
		oram.Read(buffer, blockID);
		file.write(buffer, writeLength);
		
		printf("\r%d / %d", i/CHUNK, length/CHUNK);
		fflush(stdout);
	}
	
	puts("");
	
	file.close();
}
