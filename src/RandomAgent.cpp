#include "RandomAgent.hpp"
#include "Agent.hpp"
#include "AES.hpp"

RandomAgent::RandomAgent(Agent *agent, size_t bufferSize)
: agent(agent), bufferSize(bufferSize)
{}

RandomAgent::~RandomAgent()
{}

void RandomAgent::FillBuffer()
{
	while (buffer.size() < bufferSize) {
		int64_t bid = rand() % agent->GetBlockCount();

		block data = agent->Access(Agent::READ, bid, {});

		ChangedMap changed = agent->LoadChangedMap();
		changed[bid] = 0;
		agent->SaveChangedMap(changed);

		buffer[bid] = data;
	}
}

int64_t RandomAgent::GetBlock()
{
	ChangedMap changed = agent->LoadChangedMap();
	
	// Assumes atleast 1 block hasn't been changed
	for (;;) {
		// Pick a random element
		auto iter = buffer.begin();
		std::advance(iter, rand() % buffer.size());
		
		size_t bid = iter->first;

		if (changed[bid] == 1) {
			// The data in the buffer is old
			buffer.erase(iter);

			// We've acknowledged the data has changed
			changed[bid] = 0;
		} else {
			// Save any changes we may have made
			agent->SaveChangedMap(changed);
		
			return bid;
		}
	}
}


void RandomAgent::Run()
{
	FillBuffer();

	int64_t bid = GetBlock();
	agent->Duplicate(bid, buffer[bid]);
}
