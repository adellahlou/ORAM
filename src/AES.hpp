#pragma once

#include <array>
#include <cstdint>

using raw_t = uint8_t;

template <long unsigned int N>
using raw = std::array<raw_t, N>;

class AES {
public:
	static void Setup();
	static void Cleanup();
	
	static int Encrypt(raw<32> key, raw<16> iv, raw_t *plaintext, int plen, raw_t *ciphertext);
	static int Decrypt(raw<32> key, raw<16> iv, raw_t *ciphertext, int clen, raw_t *plaintext);
	
	static int GetCiphertextLength(int plen);
};
