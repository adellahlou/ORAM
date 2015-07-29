#pragma once

#include "AES.hpp"

#include <fstream>

class File {
public:
	static void Read(std::fstream &file, raw_t *data, size_t len);
	static void Write(std::fstream &file, raw_t *data, size_t len);

	static size_t GetLength(std::fstream &file);
};
