#include "Device.hpp"

Device::Device(Agent &agent, size_t bufferSize)
: agent(agent), bufferSize(bufferSize)
{}

Device::~Device()
{}


void Device::Update()
{
	FillBuffer();

	size_t index = rand() % buffer.size();


}

void Device::FillBuffer()
{
	while (buffer.size() < bufferSize) {
		int64_t bid = rand() % agent.GetBlockCount();

		// changed[bid] = false
		
		// Should we allow having the same block twice?
		
		block data = agent.Access(Agent::READ, bid, {});


	}

}

