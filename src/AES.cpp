#include "AES.hpp"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void AES::Setup()
{
	// Initialise OpenSSL
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
}

void AES::Cleanup()
{
	EVP_cleanup();
	ERR_free_strings();
}

static void error(std::string msg)
{
	fprintf(stderr, "fatal error: %s\n", msg.c_str());
	ERR_print_errors_fp(stderr);
	exit(1);
}

int AES::Encrypt(raw<32> key, raw<16> iv, raw_t *plaintext, int plen, raw_t *ciphertext)
{
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	
	if (!ctx) {
		error("Failed to create new cipher");
	}
	
	// Initialise the encryption operation
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()) != 1) {
		error("Failed to initialise encryption");
	}
	
	// Encrypt
	int len;
	if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plen) != 1) {
		error("Failed to complete EncryptUpdate");
	}
	
	int clen = len;
	
	if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
		error("Failed to complete EncryptFinal");
	}
	clen += len;
	
	EVP_CIPHER_CTX_free(ctx);
	
	return clen;
}

int AES::Decrypt(raw<32> key, raw<16> iv, raw_t *ciphertext, int clen, raw_t *plaintext)
{
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	
	if (!ctx) {
		error("Failed to create new cipher");
	}
	
	// Initialise the decryption operation
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()) != 1) {
		error("Failed to initialise decryption");
	}
	
	// Dencrypt
	int len;
	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, clen) != 1) {
		error("Failed to complete DecryptUpdate");
	}
	
	int plen = len;
	
	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
		error("Failed to complete DecryptFinal");
	}
	plen += len;
	
	EVP_CIPHER_CTX_free(ctx);
	
	return plen;
}

// Gets the length of the corresponding ciphertext
// given the length of a plaintext
int AES::GetCiphertextLength(int plen)
{
	return (plen/16 + 1) * 16;
}

raw<16> AES::GenerateIV()
{
	raw<16> iv;
	
	if (RAND_bytes(iv.data(), iv.size()) != 1) {
		error("Need more entropy");
	}
	
	return iv;
}