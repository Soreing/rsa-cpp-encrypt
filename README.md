# rsa-cpp-encrypt

# Description

rsa-cpp-encrypt is a simple textbook RSA library for C++. The library is capable of generating 2048 bit RSA key pairs, encrypting, decrypting and also exporting/importing keys in PKCS#1 format. The implementation uses [biging-x86cpp](https://github.com/Soreing/bigint-x86cpp/)

# Installation 
Add the folders `bigint` and `rsa-crypt` from `/include` in your include path. If you want to compile the library from source, include `ASN1.cpp` and `RSACipher.cpp` from the `/src` folder. Alternatively, you can compile the source code to a static library and include it that way.

# Usage
## Creating an RSA Cipher
To create a cipher, you can provide a public or a private key. If you want to generate a new key, you can use the `genPrivKey()` function. The function generates a random key you can seed with the C `srand()` function.
```c++
srand(1234);
RSACipher rsa(genPrivKey());
```

## Importing Keys
Alternatively, you can create an empty RSA cipher, then import a public or a private key. The import and export functions only take PKCS#1 format RSA keys.
```c++
RSACipher rsa;
rsa.importPubKey(
    "-----BEGIN RSA PUBLIC KEY-----\n"
    "MIIBCgKCAQEAq ...... 7dcERQIDAQAB"
    "\n-----END RSA PUBLIC KEY-----"
);
```

```c++
RSACipher rsa;
rsa.importPrvKey(
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpQIBAAKCA ...... dMbKLovrxqU="
    "\n-----END RSA PRIVATE KEY-----"
);
```

## Exporting Keys
You can export a private or a public key into a character array buffer. You also need to provide the size of the buffer. The key is exported in PKCS#1 format
```c++
char buff[2048];
rsa.exportPubKey(buff, 2048);

// Prints PKCS#1 formatted Public Key
cout << buff << endl;
```

## Encrypting and Decrypting
Once you have created an RSA cipher, you can encrypt or decrypt 256 bytes of data. For this, the cipher expects a 256 byte large Big Integer. The result of encryption and decryption is also a Big Integer.
```c++
BigInt<256> message("123456");
BigInt<256> ciphertext = rsa.encrypt(message);
BigInt<256> plaintext  = rsa.decrypt(ciphertext);

cout<< message << "\n\n";
cout<< ciphertext.toString(64) << "\n\n";
cout<< plaintext << "\n\n";
```
```
Output:
123456

S12DtydRI2r72ouUWsHSVat5lfuU45bYVl+W3aPgcKnnlX05w2qmlgjN+MMsURnmA0/SAM+ZPvBej8Vvs3i0XwT5J6Q+qR6k7SQZ6qdPM8tiqaHeJ+9tVMLu+NJ4NTmtUypFxIufm0agvx3RyK4EmhOOw7dZC9CqadnAWxDYDpG6eVkH+PO2qUzxVddy4Iojj/29kjo2p3TzvSoE7lMoAT3trW3Er+vhjRpK/saFD8EnsTc//bfQ87EFUxt81p+e9uGiXz4Cd6aWUTXNypo3uy+VaLg9+9yQ1WVdxPrv/OMFNsE7XsFW9DgfZHGZy+4T0IekUvEjGf2+2b8ciYXpZA==

123456
```