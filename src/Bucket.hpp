#pragma once

#include "Storage.hpp"
#include "AES.hpp"

constexpr size_t BucketCipher = IV + (sizeof (Bucket)/16 + 1)*16;

class BucketHelper {
public:
	static int Encrypt(raw<Key> key, Bucket &b, raw<BucketCipher> &ciphertext);
	static int Decrypt(raw<Key> key, raw<BucketCipher> &ciphertext, Bucket &b);
};
