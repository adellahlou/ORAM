#include "ORAM.hpp"
#include "Timer.hpp"
#include "FileStorage.hpp"
#include "File.hpp"

#include <functional>
#include <fstream>

#include <cstdio>

int MBtoDepth(size_t mb);
double Profile(std::function<void()> fun);
void Write(ORAM &oram, FileInfo info);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fputs("requires a depth argument", stderr);
		return 1;
	}
	
	AES::Setup();
	
	//size_t mb = 20;
	//size_t depth = MBtoDepth(mb);
	//printf("%zuMB = depth %zu\n", mb, depth);
	
	size_t depth = strtol(argv[1], nullptr, 10);
	printf("depth = %zu\n", depth);
	
	ORAM oram(depth, {21});
	printf("#blocks = %d\n", oram.GetBlocks());
	
	FileStorage files(oram);
	files.Load();
	
	std::string filename = "benchmark/Image.ppm";
	
	double elapsedTime = 0;
	
	for (int i = 1; i <= 10; i++) {
		double time = Profile([&]() {
			files.Add("benchmark/tiny/tiny" + std::to_string(i) + ".bin");
			
			if (!files.Add(filename)) {
				// File already exists
				FileInfo info = files.GetFileInfo(filename);
			
				// Retrieve the file
				Write(oram, info);
			}
		});
		
		elapsedTime += time;
	}

	printf("\nAverage time = %f\n", elapsedTime/10.0);
	
	files.Save();
	
	AES::Cleanup();
}

void Write(ORAM &oram, FileInfo info)
{
    puts("writing");
    
    std::fstream file;
    file.open("output.ppm", std::fstream::out | std::fstream::binary | std::fstream::trunc);
            
    for (size_t i = 0; i < info.length; i += CHUNK) {
        int writeLength = std::min(CHUNK, info.length - i);
        int blockID = info.blocks[i/CHUNK];
        
        Chunk buffer;
        oram.Access(ORAM::READ, buffer, blockID);
        file.write((char *) buffer.data(), writeLength);
        
        printf("\r%zu / %zu", i/CHUNK + 1, info.length/CHUNK);
        fflush(stdout);
    }
    file.close();
    puts("\n");
}

int MBtoDepth(size_t mb)
{
	mb *= 1024*1024;
	
	size_t blocks = ceil(mb/(double) CHUNK);
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
