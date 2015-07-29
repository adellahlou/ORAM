#include "ORAM.hpp"

#include "AES.hpp"
#include "File.hpp"

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <fstream>

size_t read(ORAM &oram);
void write(ORAM &oram, size_t length);

void encrypt();

int main()
{
	ORAM oram(6);
	printf("total blocks = %d\n", oram.GetBlocks());

	size_t length = read(oram);
	
	for (size_t i = 0; i < length; i += CHUNK) {
		Chunk chunk;
		
		oram.Read(chunk, i/CHUNK);
		
		// Invert bits
		for (size_t j = 0; j < CHUNK; j++) {
			unsigned char c = chunk[j];
			chunk[j] = ~c;
		}
		
		oram.Write(chunk, i/CHUNK);
	}
	
	write(oram, length);
}

size_t read(ORAM &oram)
{
	puts("reading");
	
	std::fstream file;
	file.open("input.txt", std::fstream::in | std::fstream::binary);
	
	size_t length = File::GetLength(file);
	
	for (size_t i = 0; i < length; i += CHUNK) {
		int readLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
	
		Chunk buffer;
		file.read(buffer.data(), readLength);
		oram.Write(buffer, blockID);
	
		printf("\r%d / %zu", blockID + 1, length/CHUNK);
		fflush(stdout);
	}
	
	file.close();
	puts("\n");
	
	return length;
}

void write(ORAM &oram, size_t length)
{
	puts("writing");
	
	std::fstream file;
	file.open("output.txt", std::fstream::out | std::fstream::binary | std::fstream::trunc);
		
	for (size_t i = 0; i < length; i += CHUNK) {
		int writeLength = std::min(CHUNK, length - i);
		int blockID = i/CHUNK;
		
		Chunk buffer;
		oram.Read(buffer, blockID);
		file.write(buffer.data(), writeLength);
		
		printf("\r%d / %zu", blockID + 1, length/CHUNK);
		fflush(stdout);
	}
	file.close();
	puts("\n");
}

void encryption()
{
	AES::Setup();
	
	raw<32> key = {0};
	raw<16> iv;
	
	raw_t *plaintext = (raw_t *) "What kind of Pokemon are you?";
	int plen = strlen((char *)plaintext) + 1;
	
	raw_t ciphertext[AES::GetCiphertextLength(plen)];	
	
	int clen = AES::Encrypt(key, iv, plaintext, plen, ciphertext);
	
	raw_t output[clen];
	int olen = AES::Decrypt(key, iv, ciphertext, clen, output);
	
	printf("plen = %d\t clen = %d\tolen = %d\n", plen, clen, olen);
	puts((const char *) output);
	AES::Cleanup();
}
