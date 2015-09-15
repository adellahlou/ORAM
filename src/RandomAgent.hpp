#pragma once

#include "BlockStore.hpp"

#include <unordered_map>
#include <utility>

class Agent;

class RandomAgent {
	Agent *agent;
	
	size_t bufferSize;
	std::unordered_map<int64_t, block> buffer;

	void FillBuffer();
	int64_t GetBlock();

public:
	RandomAgent(Agent *agent, size_t bufferSize);
	~RandomAgent();

	void Run();
};
