#pragma once

#include "Types.hpp"
#include "AES.hpp"

constexpr size_t BucketCipher = IV + (sizeof (Bucket)/16 + 1)*16;

class BucketHelper {
public:
	static int Encrypt(bytes<Key> key, Bucket &b, bytes<BucketCipher> &ciphertext);
	static int Decrypt(bytes<Key> key, bytes<BucketCipher> &ciphertext, Bucket &b);
};
