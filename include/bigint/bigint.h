#ifndef SOREING_BIGINT_H
#define SOREING_BIGINT_H

#if defined(_MSC_VER)
    #define _MSVC_INTEL
#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__)
    #define _GAS_ATT
#endif

#if defined(_M_X64) ||  defined(__amd64__) || defined(__LP64__) || defined(_LP64)
    #define _X64
#else
    #define _X32
#endif

#include <iostream>
#include <string>
#include <string.h>
#include <math.h>

typedef std::string str;
typedef const char* cstr;

static const char b16[16] =
{	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static const char b64[64] =
{	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/' };

static const short v64[256] =
{	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

template <unsigned int N>
class BigInt
{
public: 
	unsigned char bytes[N];

	// Constructors and Assignment
private:
	void base2(const cstr num);
	void base8(const cstr num);
	void base10(const cstr num);
	void base16(const cstr num);
	void base64(const cstr num);
public:
	BigInt<N>();
	BigInt<N>(const cstr num, int base);
	BigInt<N>(const cstr num);
	BigInt<N>(const void* mem);
	BigInt<N>(const unsigned long long val);
	BigInt<N>(const BigInt<N> &other);
	BigInt<N>& operator=(const BigInt<N> &other);


	// Bitwise Operators
	BigInt<N> operator&(const BigInt<N> &right) const;
	BigInt<N> operator|(const BigInt<N> &right) const;
	BigInt<N> operator^(const BigInt<N> &right) const;
	BigInt<N> operator<<(const int count) const;
	BigInt<N> operator >> (const int count) const;
	BigInt<N>& operator&=(const BigInt<N> &right);
	BigInt<N>& operator|=(const BigInt<N> &right);
	BigInt<N>& operator^=(const BigInt<N> &right);
	BigInt<N>& operator<<=(const int count);
	BigInt<N>& operator>>=(const int count);
	BigInt<N> operator~() const;
	int byteCount() const;
	int bitCount() const;


	// Relational Operators
	bool isZero() const;
	bool operator>(const BigInt<N> &right) const;
	bool operator<(const BigInt<N> &right) const;
	bool operator==(const BigInt<N> &right) const;
	bool operator>=(const BigInt<N> &right) const;
	bool operator<=(const BigInt<N> &right) const;
	bool operator!=(const BigInt<N> &right) const;


	// Logical Operators
	bool operator&&(const bool other) const;
	bool operator||(const bool other) const;
	bool operator!() const;


	// Increment and Decrement Operators
	BigInt<N>& operator++();
	BigInt<N>& operator--();
	BigInt<N> operator++(int);
	BigInt<N> operator--(int);

	// Arithmetic Operators 
	BigInt<N> operator+() const;
	BigInt<N> operator-() const;
	BigInt<N> operator+(const BigInt<N> &right) const;
	BigInt<N> operator-(const BigInt<N> &right) const;
	BigInt<N> operator*(const BigInt<N> &right) const;
	BigInt<N> operator/(const BigInt<N> &right) const;
	BigInt<N> operator%(const BigInt<N> &right) const;
	BigInt<N>& operator+=(const BigInt<N> &right);
	BigInt<N>& operator-=(const BigInt<N> &right);
	BigInt<N>& operator*=(const BigInt<N> &right);
	BigInt<N>& operator/=(const BigInt<N> &right);
	BigInt<N>& operator%=(const BigInt<N> &right);

	// Extra Arithmetic Functions
	static void div(const BigInt<N> &left, const BigInt<N> &right, BigInt<N> *qptr, BigInt<N> *rptr);
	//pow<N>()


	// Type Casting Operators
	operator bool() const
	{	return !isZero();
	}

	// Stream and String Operators
public:
	str str2()  const;
	str str8()  const;
	str str10() const;
	str str16() const;
	str str64() const;
public:
	int length() const;
	str toString(int base = 10, bool prefix = false) const;

	friend std::ostream& operator<<(std::ostream &o, const BigInt<N> num)
	{	o << num.str10();
		return o;
	}

	friend std::istream& operator >> (std::istream &i, BigInt<N> &num)
	{	str input;
		i >> input;
		num = input.c_str();
		return i;
	}

};

#include "bigint_constructors.cpp"
#include "bigint_bitwise.cpp"
#include "bigint_relational.cpp"
#include "bigint_logical.cpp"
#include "bigint_arithmetic.cpp"
#include "bigint_printing.cpp"
#include "bigint_extra.cpp"

#endif