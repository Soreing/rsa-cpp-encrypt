#ifndef CRYPTOPRIME_H
#define CRYPTOPRIME_H

#include "CryptoBase.h"

template <unsigned int N>
int shiftCount(const BigInt<N> &prime);

// Deterministinc fast prime checking using a list of small primes
template <unsigned int N>
bool isPrimeFast(const BigInt<N> &prime);

// Probabilistic slow prime checking using miller rabin algorithm
template <unsigned int N>
bool isPrimeMR(const BigInt<N> &prime, int precision);

// Main primality testing function
template <unsigned int N>
bool isPrime(const BigInt<N> &prime, int precision);

#include "CryptoPrime.cpp"

#endif