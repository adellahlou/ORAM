#include "PositionMap.hpp"
#include "File.hpp"

#include <fstream>

PositionMap::PositionMap(std::string filename, int size)
: filename(filename), size(size)
{
	position = new int[size];
	
	// Read to file
	std::fstream file;
	file.open(filename, std::fstream::in | std::ios::binary);	
	
	size_t length = File::GetLength(file);
	
	alreadyExisted = length == size*sizeof (int);
	
	if (alreadyExisted) {
		file.read((char *) position, size*sizeof (int));
	}
	
	file.close();
}

PositionMap::~PositionMap()
{
	// Write to file
	std::fstream file;
	file.open(filename, std::fstream::out | std::fstream::trunc | std::ios::binary);
	
	file.write((char *) position, size*sizeof (int));
	file.close();
	
	delete[] position;
}

int &PositionMap::operator[](size_t i)
{
	return position[i];
}

bool PositionMap::AlreadyExisted()
{
	return alreadyExisted;
}
