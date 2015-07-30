#pragma once

#include "Storage.hpp"

#include <array>
#include <cstdint>

class AES {
public:
	static void Setup();
	static void Cleanup();
	
	static int Encrypt(raw<32> key, raw<16> iv, raw_t *plaintext, int plen, raw_t *ciphertext);
	static int Decrypt(raw<32> key, raw<16> iv, raw_t *ciphertext, int clen, raw_t *plaintext);
	
	static int GetCiphertextLength(int plen);
	static raw<16> GenerateIV();
};
