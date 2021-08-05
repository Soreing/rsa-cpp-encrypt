#include <rsa-crypt/RSAcipher.h>
#include <rsa-crypt/ANS1.h>

void main()
{
	RSACipher rsa(genPrivKey());
	
	BigInt<256> message("123456");
	BigInt<256> ciphertext = rsa.encrypt(message);
	BigInt<256> plaintext  = rsa.decrypt(ciphertext);

	std::cout<< message <<"\n";
	std::cout<< ciphertext <<"\n";
	std::cout<< plaintext <<"\n";

}