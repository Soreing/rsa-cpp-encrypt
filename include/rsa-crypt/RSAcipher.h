#ifndef RSACIPHER_H
#define RSACIPHER_H

#include "CryptoPrime.h"

struct RSAPublicKey
{
    BigInt<256> modulus;
	BigInt<256> publicExponent;
};

struct RSAPrivateKey
{
	BigInt<256> modulus;
	BigInt<256> publicExponent;
	BigInt<256> privateExponent;
	BigInt<256> prime1;
	BigInt<256> prime2;
	BigInt<256> exponent1;
	BigInt<256> exponent2;
	BigInt<256> coefficient;
};

// Generates a new RSA Private Key
RSAPrivateKey genPrivKey();

// Gets the Public Key data from a Private Key
RSAPublicKey  getPublicKey(const RSAPrivateKey &prk);


class RSACipher
{
private:
	RSAPublicKey publicKey;
	RSAPrivateKey privateKey;
	MontgomeryDomain<256> domain;

public:
	RSACipher();
	RSACipher(RSAPublicKey pu);
	RSACipher(RSAPrivateKey pr);

	// Generates new Private and Public key
	void generate();
	// Imports Public/Private keys from base64
	void importPubKey(const char* data);
	void importPrvKey(const char* data);
	// Exports Public/Private keys to base64
	void exportPubKey(char* buffer, size_t size);
	void exportPrvKey(char* buffer, size_t size);

	// Encrypts data (256 Bytes)
	BigInt<256> encrypt(const BigInt<256> &data);
	// Decrypts data (256 Bytes)
	BigInt<256> decrypt(const BigInt<256> &data);
};


#endif