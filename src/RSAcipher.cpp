#include <rsa-crypt/ANS1.h>
#include <rsa-crypt/RSAcipher.h>
#include <string>


RSACipher::RSACipher()
{
}

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
	BigInt<256> phiN = (pkey.prime1-one) * (pkey.prime2-one);

	// Assert publicExponent is co-prime to Phi(n)
	pkey.publicExponent = 65537;
	while (true)
	{	if (crypto_gcd(phiN, pkey.publicExponent) == one)
		{	break;
		}
		pkey.publicExponent += 2;
	}

	pkey.privateExponent = crypto_inverse(pkey.publicExponent, phiN);

	pkey.exponent1   = pkey.privateExponent % (pkey.prime1-one);
	pkey.exponent2   = pkey.privateExponent % (pkey.prime2-one);
	pkey.coefficient = crypto_inverse(pkey.prime2, pkey.prime1);

	return pkey;
}

void RSACipher::importPubKey(const char* data)
{
	static const std::string begin = "-----BEGIN RSA PUBLIC KEY-----\n";
	static const std::string end = "\n-----END RSA PUBLIC KEY-----";
	std::string pem = data; 

	size_t bSize = begin.length();
	size_t eSize = end.length();
	size_t dSize = pem.length();
	size_t bIdx  = pem.find(begin);
	size_t eIdx  = pem.find(end);

	char* intptr;
	size_t intsize;

	char* readPtr;
	size_t bytesRead;

	char buffer[2048];
	memset(buffer, 0, 2048);

	if(bIdx == 0 && eIdx == pem.length()-end.length())
	{	
		readBase64(buffer, 2048, data + bSize, dSize-bSize-eSize);
		readPtr = buffer + (buffer[1] >= 0 ? 2 : (2 + (buffer[1]&0x7F)));

		bytesRead = getInt(readPtr, 1000, intptr, intsize);
		publicKey.modulus = BigInt<256>(intptr, intsize, true);
		readPtr += bytesRead;

		bytesRead = getInt(readPtr, 1000, intptr, intsize);
		publicKey.publicExponent = BigInt<256>(intptr, intsize, true);
		readPtr += bytesRead;

		domain = MontgomeryDomain<256>(publicKey.modulus);
		privateKey = RSAPrivateKey();
	}
}

void RSACipher::importPrvKey(const char* data)
{
	static const std::string begin = "-----BEGIN RSA PRIVATE KEY-----\n";
	static const std::string end = "\n-----END RSA PRIVATE KEY-----";
	std::string pem = data; 

	size_t bSize = begin.length();
	size_t eSize = end.length();
	size_t dSize = pem.length();
	size_t bIdx  = pem.find(begin);
	size_t eIdx  = pem.find(end);

	char* intptr;
	size_t intsize;

	char* readPtr;
	size_t bytesRead;

	char buffer[4086];
	memset(buffer, 0, 4086);

	BigInt<256>* targets[8] = {
		&privateKey.modulus,
		&privateKey.publicExponent,
		&privateKey.privateExponent,
		&privateKey.prime1,
		&privateKey.prime2,
		&privateKey.exponent1,
		&privateKey.exponent2,
		&privateKey.coefficient,
	};

	if(bIdx == 0 && eIdx == pem.length()-end.length())
	{	
		readBase64(buffer, 4086, data + bSize, dSize-bSize-eSize);
		readPtr = buffer + (buffer[1] >= 0 ? 2 : (2 + (buffer[1]&0x7F)));

		bytesRead = getInt(readPtr, 1000, intptr, intsize);
		if(*intptr != 0) {return;}
		readPtr += bytesRead;

		for(int i=0; i<8; i++)
		{
			bytesRead = getInt(readPtr, 1000, intptr, intsize);
			*targets[i] = BigInt<256>(intptr, intsize, true);
			readPtr += bytesRead;
		}

		domain = MontgomeryDomain<256>(privateKey.modulus);
		publicKey = getPublicKey(privateKey);
	}
}

void RSACipher::exportPubKey(char* buffer, size_t size)
{
	char temp[2048];
	size_t tempSize  = 2048;
	size_t writeSize = 0;
	size_t objSize   = 0;

	auto n = &publicKey.modulus;
	auto e = &publicKey.publicExponent;

	writeSize = putInt((char*)(e->bytes), e->length(), temp, tempSize, false);
	objSize  += writeSize;
	tempSize -= writeSize;

	writeSize = putInt((char*)(n->bytes), n->length(), temp, tempSize, false);
	objSize  += writeSize;
	tempSize -= writeSize;

	// Paste size into the buffer
	int sSize = 0;
	for(int bCopy=objSize; bCopy>0; bCopy>>=8)
	{	temp[2048-1-objSize] = bCopy &0xFF;
		objSize++;
		sSize++;
	}

	// Add size size if it's more than one byte
	if(sSize>1 || temp[2048-objSize]<0)
	{	temp[2048-1-objSize] = 0x80 + sSize;
		objSize++;
	}

	// Add Structure tag
	temp[2048-1-objSize] = 48;
	objSize++;

	memcpy(buffer, "-----BEGIN RSA PUBLIC KEY-----\n", 31);
	int b = writeBase64(temp+2048-objSize, objSize, buffer+31, size-31);
	memcpy(buffer+31+b, "\n-----END RSA PUBLIC KEY-----\0", 30);
}

void RSACipher::exportPrvKey(char* buffer, size_t size)
{
	char temp[2048];
	size_t tempSize  = 2048;
	size_t writeSize = 0;
	size_t objSize   = 0;

	BigInt<256>* details[8] = {
		&privateKey.coefficient,
		&privateKey.exponent2,
		&privateKey.exponent1,
		&privateKey.prime2,
		&privateKey.prime1,
		&privateKey.privateExponent,
		&privateKey.publicExponent,
		&privateKey.modulus
	};

	for(int i=0; i<8; i++)
	{	writeSize = putInt((char*)(details[i]->bytes), details[i]->length(), temp, tempSize, false);
		objSize  += writeSize;
		tempSize -= writeSize;
	}

	char version=0;
	writeSize = putInt(&version, 1, temp, tempSize, false);
	objSize  += writeSize;
	tempSize -= writeSize;


	// Paste size into the buffer
	int sSize = 0;
	for(int bCopy=objSize; bCopy>0; bCopy>>=8)
	{	temp[2048-1-objSize] = bCopy &0xFF;
		objSize++;
		sSize++;
	}

	// Add size size if it's more than one byte
	if(sSize>1 || temp[2048-objSize]<0)
	{	temp[2048-1-objSize] = 0x80 + sSize;
		objSize++;
	}

	// Add Structure tag
	temp[2048-1-objSize] = 48;
	objSize++;

	memcpy(buffer, "-----BEGIN RSA PRIVATE KEY-----\n", 32);
	int b = writeBase64(temp+2048-objSize, objSize, buffer+32, size-32);
	memcpy(buffer+32+b, "\n-----END RSA PRIVATE KEY-----\0", 31);
}