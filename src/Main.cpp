#include "ORAM.hpp"
#include "Timer.hpp"
#include "FileSystem.hpp"
#include "File.hpp"
#include "Agent.hpp"
#include "RAMStore.hpp"
#include "FileStore.hpp"
#include "RandomAgent.hpp"
#include "Log.hpp"

#include <functional>
#include <fstream>
#include <cstdio>
#include <cstring>

int MBtoDepth(size_t mb, size_t blockSize = 4096);
double Profile(std::function<void()> fun);

void WriteFile(FileSystem &files, std::string inputFile, std::string outputFilename);

enum StoreType {
	FILE_TYPE,
	MEM_TYPE,

	ORAM_TYPE,
	RA_TYPE
};

int main(int argc, char **argv)
{	
	if (argc != 4) {
		printf("%s requires 3 arguments", argv[0]);
		exit(0);
	}

	AES::Setup();

	srand(time(NULL));
	
	bytes<Key> key {0};
	
	// Retrive the depth of the tree
	size_t depth = strtol(argv[3], nullptr, 10);
	printf("depth = %zu\n", depth);

	size_t blockSize = 1024*16; // 16KiB
	size_t blockCount = Z*(pow(2, depth + 1) - 1);
	
	printf("block count = %zu, block size = %zu\n", blockCount, blockSize);

	size_t storeBlockSize = 0;
	size_t storeBlockCount = 0;

	// What's backing the secure store
	StoreType storeType = MEM_TYPE;
	
	if (strcmp(argv[1], "memory") == 0) {
		storeType = MEM_TYPE;
	} else if (strcmp(argv[1], "file") == 0) {
		storeType = FILE_TYPE;
	} else {
		Log::Write(Log::WARNING, "Using memory as default");
	}
	
	// Parse the secure store type (ORAM or Agent)
	StoreType secureType = ORAM_TYPE;

	if (strcmp(argv[2], "ORAM") == 0) {
		secureType = ORAM_TYPE;
	} else if (strcmp(argv[2], "RA") == 0) {
		secureType = RA_TYPE;
	} else {
		Log::Write(Log::WARNING, "Using ORAM as default");
	}

	switch (secureType) {
		case ORAM_TYPE:
			storeBlockSize = IV + AES::GetCiphertextLength(Z*(sizeof (int32_t) + blockSize));
			storeBlockCount = blockCount/4;
			break;

		case RA_TYPE:
			storeBlockSize = IV + AES::GetCiphertextLength(blockSize);
			storeBlockCount = 2*blockCount;

			break;

		default:
			Log::Write(Log::FATAL, "Secure store type missing");
	}

	// Create the store
	BlockStore *store = nullptr;
	
	switch (storeType) {
		case FILE_TYPE:
			store = new FileStore("block.bin", storeBlockCount, storeBlockSize);
			break;

		case MEM_TYPE:
			store = new RAMStore(storeBlockCount, storeBlockSize);
			break;

		default:
			Log::Write(Log::FATAL, "Store type missing");
	}
	
	// Create the secure store
	BlockStore *secureStore = nullptr;

	switch (secureType) {
		case ORAM_TYPE:
			secureStore = new ORAM(store, depth, blockSize, key);
			break;

		case RA_TYPE:
			secureStore = new Agent(store, blockCount, blockSize, key);
			break;

		default:
			Log::Write(Log::FATAL, "Secure store type missing");
	}

	printf("#blocks = %zu\n", secureStore->GetBlockCount());

	/*
	if (!secureStore->WasSerialised()) {
		puts("zeroing blocks");

		for (size_t i = 0; i < secureStore->GetBlockCount(); i++) {
			printf("\rintialising %zu/%zu", i+1, secureStore->GetBlockCount());
			fflush(stdout);

			block b(blockSize, i % 256);
			secureStore->Write(i, b);
		}
		puts("");
	}
	*/

	/*
	FileSystem files(secureStore);
	files.Load();
	
	std::string input = "input.bin";
	std::string output = "output.bin";
		
	Profile([&]() {
		// Write the file if it already exists
		if (!files.Add(input)) {
			WriteFile(files, input, output);
		}
	});
	
	files.Save();
	*/

	//block b(secureStore->GetBlockSize(), 17);
	//secureStore->Write(0, b);

	//block b = secureStore->Read(0);
	//printf("block contains %d\n", (int) b[0]);

	
	
	delete secureStore;
	delete store;

	AES::Cleanup();
}

void WriteFile(FileSystem &files, std::string inputFilename, std::string outputFilename)
{
	BlockStore *store = files.GetBlockStore();

	// Retrieve metadata
	FileInfo info = files.GetFileInfo(inputFilename);

	// Open output file
	std::fstream file;
	file.open(outputFilename, std::ios::out | std::ios::binary | std::ios::trunc);

	size_t blockSize = store->GetBlockSize();

	for (size_t i = 0; i < info.length; i += blockSize) {
		size_t writeLength = std::min(blockSize, info.length - i);
		size_t pos = info.blocks[i/blockSize];

		// Read from the store
		block buffer = store->Read(pos);

		// Write it to file
		file.write((char *) buffer.data(), writeLength);

		printf("\r%zu / %zu", i/blockSize + 1, info.length/blockSize);
		fflush(stdout);
	}
	puts("\n");

	file.close();
}

int MBtoDepth(size_t mb, size_t blockSize /*= 4096*/)
{
	mb *= 1024*1024;
	
	size_t blocks = ceil(mb/(double) blockSize);
	size_t buckets = ceil(blocks/(double) Z);
	
	return ceil(log2(buckets+1))-1;
}

double Profile(std::function<void()> fun)
{
	Timer timer;
	timer.Start();
	
	fun();
	
	timer.Stop();
	double elapsedTime = timer.GetElapsedTime();
	
	printf("Elapsed time = %f\n", elapsedTime);
	
	return elapsedTime;
}
