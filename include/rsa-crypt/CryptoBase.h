#ifndef CRYPTOBASE_H
#define CRYPTOBASE_H

#include <bigint/bigint.h>

template <unsigned int N>
class MontgomeryDomain
{
public:
	BigInt<N * 2> mod;
	BigInt<N * 2> r;
	BigInt<N * 2> R;

	BigInt<N * 2> mask;
	int shift;

	bool fast;

public:
	MontgomeryDomain(){}
	
	// Creates a Montgomery domain and its signature values
	MontgomeryDomain(const BigInt<N> &m);

	// Transforms a normal value into Montgomery domain
	BigInt<N * 2> slow_transform(const BigInt<N> &val);
	BigInt<N * 2> fast_transform(const BigInt<N> &val);
	BigInt<N * 2> transform(const BigInt<N> &val);

	// Reverts a value from the Montgomery domain to normal
	BigInt<N> slow_revert(const BigInt<N * 2> &val);
	BigInt<N> fast_revert(const BigInt<N * 2> &val);
	BigInt<N> revert(const BigInt<N * 2> &val);

	// Squares a number that is in Montgomery domain
	BigInt<N * 2>& slow_square(BigInt<N * 2> &num);
	BigInt<N * 2>& fast_square(BigInt<N * 2> &num);
	BigInt<N * 2> square(BigInt<N * 2> &num);

	// Multiplies a number that is in Montgomery Domain
	BigInt<N * 2>& slow_multiply(BigInt<N * 2> &left, BigInt<N * 2> &right);
	BigInt<N * 2>& fast_multiply(BigInt<N * 2> &left, BigInt<N * 2> &right);
	BigInt<N * 2> multiply(BigInt<N * 2> &left, BigInt<N * 2> &right);
};

// Calculates the GCD of 2 numbers
template <unsigned int N>
BigInt<N> crypto_gcd(BigInt<N> a, BigInt<N> b);

// Calculates the Modular Multiplicative Inverse of a number
template <unsigned int N>
BigInt<N> crypto_inverse(BigInt<N> x, BigInt<N> mod);

// Modular Exponentiation of a number using a Montgomery Domain
template <unsigned int N>
void crypto_pow(BigInt<N * 2> &x, BigInt<N> &exp, MontgomeryDomain<N> &dom);

#include "CryptoBase.cpp"

#endif