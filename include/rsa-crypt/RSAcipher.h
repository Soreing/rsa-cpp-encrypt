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
	BigInt<256> phiOfModulus;
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

	void generate();
	BigInt<256> encrypt(const BigInt<256> &data);
	BigInt<256> decrypt(const BigInt<256> &data);

};

RSACipher::RSACipher(RSAPublicKey pu)
	: publicKey(pu), domain(pu.modulus)
{
}

RSACipher::RSACipher(RSAPrivateKey pr)
	: privateKey(pr), publicKey(getPublicKey(pr)), domain(pr.modulus)
{
}

void RSACipher::generate()
{	
	privateKey = genPrivKey();
	publicKey = getPublicKey(privateKey);
	domain = MontgomeryDomain<256>(publicKey.modulus);
}

BigInt<256> RSACipher::encrypt(const BigInt<256> &data)
{
	BigInt<512> message = domain.transform(data);
	crypto_pow(message, publicKey.publicExponent, domain);
	return domain.revert(message);
}

BigInt<256> RSACipher::decrypt(const BigInt<256> &data)
{
	BigInt<512> message = domain.transform(data);
	crypto_pow(message, privateKey.privateExponent, domain);
	return domain.revert(message);
}


RSAPublicKey  getPublicKey(const RSAPrivateKey &prk)
{
	return RSAPublicKey{prk.modulus, prk.publicExponent};
}

RSAPrivateKey genPrivKey()
{
	RSAPrivateKey pkey;
	BigInt<256> one(1);

	// Generate new prime numbers for p and q
	genratePrime(*(BigInt<128>*)&pkey.prime1);
	genratePrime(*(BigInt<128>*)&pkey.prime2);

	// Find the modulus n and phi(n)
	pkey.modulus = pkey.prime1 * pkey.prime2;
	pkey.phiOfModulus = (pkey.prime1-one) * (pkey.prime2-one);

	// Assert publicExponent is co-prime to Phi(n)
	pkey.publicExponent = 3;
	while (true)
	{	if (crypto_gcd(pkey.phiOfModulus, pkey.publicExponent) == one)
		{	break;
		}
		pkey.publicExponent += 2;
	}

	pkey.privateExponent = crypto_inverse(pkey.publicExponent, pkey.phiOfModulus);

	return pkey;
}