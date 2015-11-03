#pragma once

#include "BlockStore.hpp"
#include "Agent.hpp"

class Device {
	Agent &agent;
	
	std::vector<block> buffer;

	size_t bufferSize;

	void FillBuffer();
	
public:
	Device(Agent &agent, size_t bufferSize);
	~Device();

	void Update();
};
