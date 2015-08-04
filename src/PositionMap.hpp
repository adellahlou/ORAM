#pragma once

#include <string>

/*
Maps each block ID to a paths
*/
class PositionMap {
	std::string filename;

	int *position;
	const int size;
	
	bool alreadyExisted;
	
public:
	PositionMap(std::string filename, int size);
	~PositionMap();
	
	int &operator[](size_t i);
	
	bool AlreadyExisted();
};
