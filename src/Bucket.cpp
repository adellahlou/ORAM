#include "Bucket.hpp"

#include <cstring>

int BucketHelper::Encrypt(raw<Key> key, Bucket &b, raw<BucketCipher> &ciphertext)
{
	// Write the unencrypted IV
	raw<IV> iv = AES::GenerateIV();	
	memcpy(ciphertext.data(), iv.data(), iv.size());

	int plen = sizeof (Bucket);
	return AES::Encrypt(key, iv, (raw_t *) &b, plen, ciphertext.data() + iv.size());
}

int BucketHelper::Decrypt(raw<Key> key, raw<BucketCipher> &ciphertext, Bucket &b)
{
	// Retrieve the unencrypted IV
	raw<IV> iv;
	memcpy(iv.data(), ciphertext.data(), iv.size());
	
	int clen = ciphertext.size() - iv.size();
	return AES::Decrypt(key, iv, ciphertext.data() + iv.size(), clen, (raw_t *) &b);
}
