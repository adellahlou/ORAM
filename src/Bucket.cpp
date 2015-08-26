#include "Bucket.hpp"

#include <cstring>

int BucketHelper::Encrypt(bytes<Key> key, Bucket &b, bytes<BucketCipher> &ciphertext)
{
	// Write the unencrypted IV
	bytes<IV> iv = AES::GenerateIV();	
	memcpy(ciphertext.data(), iv.data(), iv.size());

	int plen = sizeof (Bucket);
	return AES::Encrypt(key, iv, (byte_t *) &b, plen, ciphertext.data() + iv.size());
}

int BucketHelper::Decrypt(bytes<Key> key, bytes<BucketCipher> &ciphertext, Bucket &b)
{
	// Retrieve the unencrypted IV
	bytes<IV> iv;
	memcpy(iv.data(), ciphertext.data(), iv.size());
	
	int clen = ciphertext.size() - iv.size();
	return AES::Decrypt(key, iv, ciphertext.data() + iv.size(), clen, (byte_t *) &b);
}
