#include "BigInt.h"
//----------------------------------------------------------------------------//
//                       Increment and Decrement Operators                    //
//--------------------------------------------------------------------------- //

// Increments a Big Integer of N bytes in Assembly. The addition stops when there is no carry.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>& BigInt<N>::operator++()
{
	const unsigned char* b = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, b;
		mov rdi, b;
		mov ecx, len;
		stc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		adc eax, 0;
		stosd;
		jc next;

	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, b;
		mov edi, b;
		mov ecx, len;
		stc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		adc eax, 0;
		stosd;
		jc next;

	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %0, %%rdi \n"
		"mov %1, %%ecx \n"
		"stc \n"

	"1: \n"
		"jecxz 1f \n"
		"dec %%ecx \n"

		"lodsl \n"
		"adc $0, %%eax \n"
		"stosl \n"
		"jc 1b \n"

	"1: \n"
	    :
        : "m" (b), "m" (len)
	);
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
        "mov %0, %%esi \n"
        "mov %0, %%edi \n"
        "mov %1, %%ecx \n"
        "stc \n"

    "1: \n"
        "jecxz 1f \n"
        "dec %%ecx \n"

        "lodsl \n"
        "adc $0, %%eax \n"
        "stosl \n"
        "jc 1b \n"

    "1: \n"
        :
        : "m" (b), "m" (len)
    );
#endif

	return *this;
}

// Decrements a Big Integer of N bytes in Assembly. The subtraction stops when there is no borrow.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>& BigInt<N>::operator--()
{
	const unsigned char* b = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, b;
		mov rdi, b;
		mov ecx, len;
		stc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		sbb eax, 0;
		stosd;
		jc next;

	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, b;
		mov edi, b;
		mov ecx, len;
		stc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		sbb eax, 0;
		stosd;
		jc next;

	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %0, %%rdi \n"
		"mov %1, %%ecx \n"
		"stc \n"

	"1: \n"
		"jecxz 1f \n"
		"dec %%ecx \n"

		"lodsl \n"
		"sbb $0, %%eax \n"
		"stosl \n"
		"jc 1b \n"

	"1: \n"
	    :
        : "m" (b), "m" (len)
	);
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
    "mov %0, %%esi \n"
    "mov %0, %%edi \n"
    "mov %1, %%ecx \n"
    "stc \n"

    "1: \n"
    "jecxz 1f \n"
    "dec %%ecx \n"

    "lodsl \n"
    "sbb $0, %%eax \n"
    "stosl \n"
    "jc 1b \n"

    "1: \n"
    :
    : "m" (b), "m" (len)
    );
#endif

	return *this;
}

// Increments a Big Integer of N bytes and returns a copy of the original.
template <unsigned int N>
BigInt<N> BigInt<N>::operator++(int)
{
	BigInt<N> res = *this;
    ++(*this);
	return res;
}

// Decrements a Big Integer of N bytes and returns a copy of the original.
template <unsigned int N>
BigInt<N> BigInt<N>::operator--(int)
{
	BigInt<N> res = *this;
    --(*this);
	return res;
}



//----------------------------------------------------------------------------//
//                             Arithmetic Operators                           //
//--------------------------------------------------------------------------- //

// Simply Returns the object
template <unsigned int N>
BigInt<N>  BigInt<N>::operator+() const
{   return *this;
}

// Negates the value of the object
template <unsigned int N>
BigInt<N>  BigInt<N>::operator-() const
{   return (~(*this))++;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the += operator add the two numbers, then return the result.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator+(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res += right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the -= operator subtract the two numbers, then return the result.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator-(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res -= right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the *= operator multiply the two numbers, then return the result.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator*(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res *= right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the /= operator divide the two numbers, then return the result.
template <unsigned int N>
BigInt<N> BigInt<N>::operator/(const BigInt<N> &right) const
{
	BigInt<N> copy(*this);
    copy /= right;
    return copy;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the %= operator get the remainder the two numbers divided, then return the result.
template <unsigned int N>
BigInt<N> BigInt<N>::operator%(const BigInt<N> &right) const
{
	BigInt<N> copy(*this);
    copy %= right;
    return copy;
}

// Swaps the operands for + when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator+(const T left, const BigInt<N> &right)
{   return right + left;
}

// Casts the left hand operand for - when it's not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator-(const T left, const BigInt<N> &right)
{   return BigInt<N>(right) - left;
}

// Swaps the operands for * when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator*(const T left, const BigInt<N> &right)
{   return right * left;
}

// Casts the left hand operand for / when it's not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator/(const T left, const BigInt<N> &right)
{   return BigInt<N>(right) / left;
}

// Casts the left hand operand for % when it's not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator%(const T left, const BigInt<N> &right)
{   return BigInt<N>(right) % left;
}

// Adds two Big Integers of N bytes in Assembly using ADC for add with carry.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator+=(const BigInt<N> &right)
{
	const unsigned char* r = right.bytes;
	unsigned char* l = bytes;
	int len = N >> 2;
	int addr=0;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, r;
		mov rdi, l;
		mov ecx, len;
		clc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		adc eax, [rdi];
		stosd;
		jmp next;

	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, r;
		mov edi, l;
		mov ecx, len;
		clc;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		adc eax, [edi];
		stosd;
		jmp next;

	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
    (
    "mov %0, %%rsi \n"
    "mov %1, %%rdi \n"
    "mov %2, %%ecx \n"
    "clc \n"

    "1: \n"
    "jrcxz 1f \n"
    "dec %%rcx \n"

    "lodsl \n"
    "adc (%%rdi), %%eax \n"
    "stosl \n"
    "jmp 1b \n"

    "1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
    "mov %0, %%esi \n"
    "mov %1, %%edi \n"
    "mov %2, %%ecx \n"
    "clc \n"

    "1: \n"
    "jecxz 1f \n"
    "dec %%ecx \n"

    "lodsl \n"
    "adc (%%edi), %%eax \n"
    "stosl \n"
    "jmp 1b \n"

    "1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#endif

    return *this;
}

// Subtracts two Big Integers of N bytes in Assembly using SBB for subtract with borrow.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator-=(const BigInt<N> &right)
{
	const unsigned char* r = right.bytes;
	unsigned char* l = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, r
		mov rdi, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		mov ebx, [rdi];
		sbb ebx, eax;
		mov eax, ebx;
		stosd;
		jmp next;
	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, r
		mov edi, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		mov ebx, [edi];
		sbb ebx, eax;
		mov eax, ebx;
		stosd;
		jmp next;
	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %1, %%rdi \n"
		"mov %2, %%ecx \n"

	"1: \n"
		"jecxz 1f \n"
		"dec %%ecx \n"

		"lodsl \n"
		"mov (%%rdi), %%ebx \n"
		"sbb %%eax, %%ebx \n"
		"mov %%ebx, %%eax \n"
		"stosl \n"
		"jmp 1b \n"
	"1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
	(
		"mov %0, %%esi \n"
		"mov %1, %%edi \n"
		"mov %2, %%ecx \n"

	"1: \n"
		"jecxz 1f \n"
		"dec %%ecx \n"

		"lodsl \n"
		"mov (%%edi), %%ebx \n"
		"sbb %%eax, %%ebx \n"
		"mov %%ebx, %%eax \n"
		"stosl \n"
		"jmp 1b \n"
	"1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#endif

	return *this;
}

// Karatsuba algorithm that multiples a byte array of N bytes by another array of N bytes.
// 3 byte arrays of 2N size are used, but declared as one array for faster access.
// Z1 is used for the Z1 part of the algorithm, while the other two arrays, t1 and t2 
// are used for various temporary values to save space.
// First the (L1+L2) * (R1+R2) part of Z1 is calculated. If an overflow occurred during
// addition of either operands, Karatsuba of the same size is called, otherwise
// Karatsuba of half the size is called, then the temporary arrays are cleared.
// Z0=L2*R2 is calculated in T1, then subtracted from Z1 and added to T2 (result) so there is
// no need for this value anymore and T1 can be cleared.
// Z2=L1*R1 is calculated in T1, then subtracted from Z1.
// Z2(T1) and Z1 are shifted and added to the result (T2), which is then copied into
// the left hand operand to simulate the *= operand.
template <unsigned int N>
void karatsuba(char* left, const char* right)
{
	if (((BigInt<N>*)left)->isZero())
		return;
	if (((BigInt<N>*)right)->isZero())
	{   memset(left, 0, N * 2);
		return;
	}

	unsigned char stack[N * 6];
	unsigned char* z1 = (unsigned char*)stack;
	unsigned char* t1 = (unsigned char*)stack + N + N;
	unsigned char* t2 = (unsigned char*)stack + N + N + N + N;
	memset(stack, 0, N * 6);

	//(L1+L2)
	memcpy(z1, left, (N >> 1));
	memcpy(t1, left + (N >> 1), (N >> 1));
	*(BigInt<N>*)z1 += *(BigInt<N>*)t1;

	//(R1+R2)
	memcpy(t1, right, (N >> 1));
	memcpy(t2, right + (N >> 1), (N >> 1));
	*(BigInt<N>*)t1 += *(BigInt<N>*)t2;

	//(L1+L2) * (R1+R2)
	if (*(z1 + (N >> 1)) | *(t1 + (N >> 1)))
		karatsuba<N>((char*)z1, (char*)t1);
	else
		karatsuba<(N >> 1)>((char*)z1, (char*)t1);

	// Clear T1 and T2
	memset(t1, 0, N * 4);

	// Z0=L2*R2 in T1
	memcpy(t1, left, (N >> 1));
	karatsuba<(N >> 1)>((char*)t1, (char*)right);
	memcpy(t2, t1, N);
	*(BigInt<N * 2>*)z1 -= *(BigInt<N * 2>*)t1;

	// Z2=L1*R1 in T1
	memset(t1 + (N >> 1), 0, (N >> 1));
	memcpy(t1, left + (N >> 1), (N >> 1));
	karatsuba<(N >> 1)>((char*)t1, (char*)(right + (N >> 1)));
	*(BigInt<N * 2>*)z1 -= *(BigInt<N * 2>*)t1;

	// Result Z2 + Z1 + Z0 (shift adjusted)
	*(BigInt<N>*)(t2 + N) += *(BigInt<N>*)t1;
	memset(t1, 0, N >> 1);
	memcpy(t1 + (N >> 1), z1, N + 1);
	*(BigInt<N * 2>*)t2 += *(BigInt<N * 2>*)t1;

	//Result
	memcpy(left, t2, N * 2);
}

// Squares a number that is N bytes long. (Must be power of 2 length, at least 8 bytes)
// Uses the square expansion (a+b)^2 = a^2 + 2ab + b^2 to efficiently calculate the result.
// [1] The integer is split in 2 halves to calculate a^2 and b^2, and copied to calculate ab.
// [2] a^2 and b^2 is calculated using squaring, while ab is calculated using karatsuba. Ex: 84^2 = 8^2 + 8*4 + 4^2
// [3] The results from Step 2 are bit adjusted. a^2 was already calculated in the correct shift.
//     ab is shifted (N/2*8) + 1 bits (+1 because of 2ab) and added to a^2 + b^2 
template <unsigned int N>
void square(char* num)
{
	char ab[N * 2];
	
// [1]
	memcpy(num + N, num + N / 2, N / 2);
	memset(num + N / 2, 0, N / 2);
	memcpy(ab, num, N * 2);

// [2]
	square<N / 2>(num);
	square<N / 2>(num + N);
	karatsuba <N / 2>((char*)ab, (char*)(ab + N));

// [3]
	memset(ab + N, 0, N / 2);
	*(BigInt<N * 2>*)ab <<= N*4+1;
	*(BigInt<N * 2>*)num += *(BigInt<N * 2>*)ab;
}

// Template Specialization of the Karatsuba function to multiply the operands
// using built in multiplication once the operands are small enough (4 bytes).
// This is used as an exit condition to the recursive Karatsuba function.
template <>
void karatsuba<4>(char* left, const char* right)
{   *(unsigned long long*)left *= *(unsigned long*)right;
    return;
}

// Template Specialization of the Square function to multiply the operands
// using built in multiplication once the operands are small enough (4 bytes).
// This is used as an exit condition to the recursive Square function.
template <>
void square<4>(char* num)
{
	*(unsigned long long*)num *= *(unsigned long*)num;
	return;
}

// Multiplies a Big Integer of N bytes with another N Byte Big Integer using Karatsuba.
// The left hand side is copied to a buffer of 2N bytes to allow for intermediate overflow.
// The right side is copied to a buffer of N bytes just to allow for sign modifications.
// Both right and left hand operands are checked for sign and converted to positive.
// The positive operands are multiplied using Karatsuba, then truncated and sign adjusted.
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator*=(const BigInt<N> &right)
{
    unsigned char left[N * 2];
    unsigned char rightCpy[N];

    memset(left + N, 0, N);
    memcpy(left, bytes, N);
    memcpy(rightCpy, &right, N);

    karatsuba<(N)>((char*)left, (char*)rightCpy);
    memcpy(bytes, left, N);

    return *this;
}

// Divides two Big Integers of N bytes using shift subtraction.
// Both the left and right operands are copied to a new array to allow for modification.
// The operands are checked for negative signs and are converted to positive.
// The divisor is shifted to the left to have the same amount of bits as the dividend.
// The divisor is compared to the dividend, and if smaller, it's subtracted, and a bit is
// appended to the result(quotient). The dividend is continuously shifted to the right in
// each iteration, until the result is shifted to the left until the divisor is back to
// its original value.
// The result(quotient) is sign adjusted and copied into the object and returned.
template <unsigned int N>
BigInt<N>& BigInt<N>::operator/=(const BigInt<N> &right)
{
	BigInt<N> quotient;
	BigInt<N> dividend(*this);
	BigInt<N> divisor(right);

	int proc = dividend.bitCount() - divisor.bitCount();
	divisor <<= proc;

	while (proc > 0)
	{
		if (divisor <= dividend)
		{   *(char*)&quotient |= 1;
			dividend -= divisor;
		}

		quotient <<= 1;
		divisor >>= 1;
		proc--;
	}

	if (divisor <= dividend)
	{   *(char*)&quotient |= 1;
		dividend -= divisor;
	}

	memcpy(bytes, &quotient, N);
	return *this;
}

// Divides two Big Integers of N bytes using shift subtraction.
// Both the left and right operands are copied to a new array to allow for modification.
// The operands are checked for negative signs and are converted to positive.
// The divisor is shifted to the left to have the same amount of bits as the dividend.
// The divisor is compared to the dividend, and if smaller, it's subtracted, and a bit is
// appended to the result(quotient). The dividend is continuously shifted to the right in
// each iteration, until the result is shifted to the left until the divisor is back to
// its original value.
// The remainder(dividend) is sign adjusted, copied into the object and returned.
template <unsigned int N>
BigInt<N>& BigInt<N>::operator%=(const BigInt<N> &right)
{
	BigInt<N> quotient;
	BigInt<N> dividend(*this);
	BigInt<N> divisor(right);

	int proc = dividend.bitCount() - divisor.bitCount();
	divisor <<= proc;

	while (proc > 0)
	{
		if (divisor <= dividend)
		{   *(char*)&quotient |= 1;
			dividend -= divisor;
		}

		quotient <<= 1;
		divisor >>= 1;
		proc--;
	}

	if (divisor <= dividend)
	{   *(char*)&quotient |= 1;
		dividend -= divisor;
	}

	memcpy(bytes, &dividend, N);
	return *this;
}

template <unsigned int N>
void BigInt<N>::div(const BigInt<N> &left, const BigInt<N> &right, BigInt<N> *qptr, BigInt<N> *rptr)
{
	BigInt<N> quotient;
	BigInt<N> dividend(left);
	BigInt<N> divisor(right);

	int proc = dividend.bitCount() - divisor.bitCount();
	divisor <<= proc;
	while (proc > 0)
	{
		if (divisor <= dividend)
		{	*(char*)&quotient |= 1;
			dividend -= divisor;
		}

		quotient <<= 1;
		divisor >>= 1;
		proc--;
	}

	if (divisor <= dividend)
	{	*(char*)&quotient |= 1;
		dividend -= divisor;
	}

	memcpy(qptr, &quotient, N);
	memcpy(rptr, &dividend, N);
}