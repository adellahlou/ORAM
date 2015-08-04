#include "ORAM.hpp"
#include "File.hpp"
#include "Timer.hpp"
#include "FileStorage.hpp"

#include <functional>

#include <cstring>
#include <cstdio>
#include <cstdlib>


int MBtoDepth(size_t mb)
{
	mb *= 1024*1024;
	
	size_t blocks = ceil(mb/(double) CHUNK);
	size_t buckets = ceil(blocks/(double) Z);
	
	return ceil(log2(buckets+1))-1;
}

void Profile(std::function<void()> fun)
{
	Timer timer;
	timer.Start();
	
	fun();
	
	timer.Stop();
	
	printf("Elapsed time = %f\n", timer.GetElapsedTime());
}

int main()
{
	AES::Setup();
	
	size_t mb = 200;
	size_t depth = MBtoDepth(mb);
	printf("%zuMB = depth %zu\n", mb, depth);
	
	ORAM oram(depth);
	
	printf("#blocks = %d\n", oram.GetBlocks());
	
	FileStorage files(oram);
	
	Profile([&]() {
		files.Add("32.png");
	});
	
	files.Remove("input.txt");
	
	AES::Cleanup();
}
