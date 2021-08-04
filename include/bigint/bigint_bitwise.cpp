
//----------------------------------------------------------------------------//
//                              Bitwise Operators                             //
//--------------------------------------------------------------------------- //

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the &= operator to modify the new object, then return it.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator&(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res &= right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the |= operator to modify the new object, then return it.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator|(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res |= right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the ^= operator to modify the new object, then return it.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator^(const BigInt<N> &right) const
{
	BigInt<N> res = *this;
	res ^= right;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the <<= operator to shift the new object, then return it.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator<<(const int count) const
{
	BigInt<N> res = *this;
	res <<= count;
	return res;
}

// Creates a new Big Integer of the same size and copies N bytes from the original.
// The implementation uses the >>= operator to shift the new object, then return it.
template <unsigned int N>
BigInt<N>  BigInt<N>::operator>>(const int count) const
{
	BigInt<N> res = *this;
	res >>= count;
	return res;
}

// Swaps the operands for & when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator&(const T left, const BigInt<N> &right)
{   return right & left;
}

// Swaps the operands for | when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator|(const T left, const BigInt<N> &right)
{   return right | left;
}

// Swaps the operands for ^ when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
BigInt<N>  operator^(const T left, const BigInt<N> &right)
{   return right ^ left;
}

// Uses the AND instruction in Assembly to bitwise & the Big Integer to another of the same size.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator&=(const BigInt<N> &right)
{
	const unsigned char* r = right.bytes;
	unsigned char* l = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, r;
		mov rdi, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		and eax, [rdi];
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

	next:
		jecxz end;
		dec ecx;

		lodsd;
		and eax, [edi];
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
		"and (%%rdi), %%eax \n"
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
        "and (%%edi), %%eax \n"
        "stosl \n"
        "jmp 1b \n"

    "1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#endif

	return *this;
}

// Uses the OR instruction in Assembly to bitwise | the Big Integer to another of the same size.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator|=(const BigInt<N> &right)
{
	const unsigned char* r = right.bytes;
	unsigned char* l = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, r;
		mov rdi, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		or eax, [rdi];
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

	next:
		jecxz end;
		dec ecx;

		lodsd;
		or eax, [edi];
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
		"or (%%rdi), %%eax \n"
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
        "or (%%edi), %%eax \n"
        "stosl \n"
        "jmp 1b \n"

    "1: \n"
        :
        : "m" (r), "m" (l), "m" (len)
    );
#endif

	return *this;
}

// Uses the XOR instruction in Assembly to bitwise ^ the Big Integer to another of the same size.
// The value of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N>&  BigInt<N>::operator^=(const BigInt<N> &right)
{
	const unsigned char* r = right.bytes;
	unsigned char* l = bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, r;
		mov rdi, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		xor eax, [rdi];
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

	next:
		jecxz end;
		dec ecx;

		lodsd;
		xor eax, [edi];
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
		"xor (%%rdi), %%eax \n"
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
		"xor (%%edi), %%eax \n"
		"stosl \n"
		"jmp 1b \n"

	"1: \n"
	    :
        : "m" (r), "m" (l), "m" (len)
	);
#endif

	return *this;
}

template <unsigned int N>
BigInt<N>&  BigInt<N>::operator<<=(const int count)
{
	unsigned char* src = bytes + N - 4 - (count / 16) * 2;
	unsigned char* dst = bytes + N - 2;
	unsigned char* end = bytes;
	unsigned char  sft = 16 - (count % 16);


	if (count >= N * 8 || count < 0)
	{
		memset(bytes, 0, N);
		return *this;
	}

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, src;
		mov rdi, dst;
		mov rdx, end;
		mov cl, sft;

	sh:
		cmp rsi, rdx;
		jl quit;

		lodsd;
		shr eax, cl;
		stosw;

		sub rsi, 6;
		sub rdi, 4;
		jmp sh;

	quit:
		add rsi, 2;
		lodsw;
		sub cl, 16;
		neg cl;
		shl ax, cl;
		stosw;
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, src;
		mov edi, dst;
		mov edx, end;
		mov cl, sft;

	sh:
		cmp esi, edx;
		jl quit;

		lodsd;
		shr eax, cl;
		stosw;

		sub esi, 6;
		sub edi, 4;
		jmp sh;

	quit:
		add esi, 2;
		lodsw;
		sub cl, 16;
		neg cl;
		shl ax, cl;
		stosw;
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %1, %%rdi \n"
		"mov %2, %%rdx \n"
		"mov %3, %%cl \n"

	"1: \n"
		"cmp %%rdx, %%rsi \n"
		"jl 1f \n"

		"lodsl \n"
		"shr %%cl, %%eax \n"
		"stosw \n"

		"sub $6, %%rsi \n"
		"sub $4, %%rdi \n"
		"jmp 1b \n"

	"1: \n"
		"add $2, %%rsi \n"
		"lodsw \n"
		"sub $16, %%cl \n"
		"neg %%cl \n"
		"shl %%cl, %%ax \n"
		"stosw \n"
		:
        : "m" (src), "m" (dst), "m" (end), "m" (sft)

	);
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
        "mov %0, %%esi \n"
        "mov %1, %%edi \n"
        "mov %2, %%edx \n"
        "mov %3, %%cl \n"

    "1: \n"
        "cmp %%edx, %%esi \n"
        "jl 1f \n"

        "lodsl \n"
        "shr %%cl, %%eax \n"
        "stosw \n"

        "sub $6, %%esi \n"
        "sub $4, %%edi \n"
        "jmp 1b \n"

    "1: \n"
        "add $2, %%esi \n"
        "lodsw \n"
        "sub $16, %%cl \n"
        "neg %%cl \n"
        "shl %%cl, %%ax \n"
        "stosw \n"
        :
        : "m" (src), "m" (dst), "m" (end), "m" (sft)
    );
#endif

	memset(bytes, 0, (count / 16) * 2);
	return *this;
}

template <unsigned int N>
BigInt<N>&  BigInt<N>::operator>>=(const int count)
{
	unsigned char* srcp = bytes + (count / 16) * 2;
	unsigned char* dstp = bytes;
	unsigned char* endp = bytes + N - 2;
	unsigned char  sft = (count % 16);


	if (count >= N * 8 || count < 0)
	{
		memset(bytes, 0, N);
		return *this;
	}

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, srcp;
		mov rdi, dstp;
		mov rdx, endp;
		mov cl, sft;

	sh:
		cmp rsi, rdx;
		jge end;

		lodsd;
		shr eax, cl;
		stosw;

		sub rsi, 2;
		jmp sh;

	end:
		lodsw;
		shr ax, cl;
		stosw;
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, srcp;
		mov edi, dstp;
		mov edx, endp;
		mov cl, sft;

	sh:
		cmp esi, edx;
		jge end;

		lodsd;
		shr eax, cl;
		stosw;

		sub esi, 2;
		jmp sh;

	end:
		lodsw;
		shr ax, cl;
		stosw;
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %1, %%rdi \n"
		"mov %2, %%rdx \n"
		"mov %3, %%cl \n"

	"1: \n"
		"cmp %%rdx, %%rsi \n"
		"jge 1f \n"

		"lodsl \n"
		"shr %%cl, %%eax \n"
		"stosw \n"

		"sub $2, %%rsi \n"
		"jmp 1b \n"

	"1: \n"
		"lodsw \n"
		"shr %%cl, %%ax \n"
		"stosw \n"
		:
        : "m" (srcp), "m" (dstp), "m" (endp), "m" (sft)
	);
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
        "mov %0, %%esi \n"
        "mov %1, %%edi \n"
        "mov %2, %%edx \n"
        "mov %3, %%cl \n"

    "1: \n"
        "cmp %%edx, %%esi \n"
        "jge 1f \n"

        "lodsl \n"
        "shr %%cl, %%eax \n"
        "stosw \n"

        "sub $2, %%esi \n"
        "jmp 1b \n"

    "1: \n"
        "lodsw \n"
        "shr %%cl, %%ax \n"
        "stosw \n"
        :
        : "m" (srcp), "m" (dstp), "m" (endp), "m" (sft)
    );
#endif

	memset(bytes + N - (count / 8), 0, (count / 8));
	return *this;
}


// Uses the NOT instruction in Assembly to flip the bits of the Big Integer.
// The vlaue of the number is iterated through 4 bytes at a time (32 bit instructions).
template <unsigned int N>
BigInt<N> BigInt<N>::operator~() const
{
	BigInt<N> res = *this;
	const unsigned char* b = res.bytes;
	int len = N >> 2;

#if defined(_MSVC_INTEL) && defined(_X64)
    __asm
	{
		mov rsi, b;
		mov rdi, b;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		not eax;
		stosd;
		jmp next;

	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov esi, b;
		mov edi, b;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		lodsd;
		not eax;
		stosd;
		jmp next;

	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %0, %%rsi \n"
		"mov %0, %%rdi \n"
		"mov %1, %%ecx \n"

	"1: \n"
		"jecxz 1f \n"
		"dec %%ecx \n"

		"lodsl \n"
		"not %%eax \n"
		"stosl \n"
		"jmp 1b \n"

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

    "1: \n"
        "jecxz 1f \n"
        "dec %%ecx \n"

        "lodsl \n"
        "not %%eax \n"
        "stosl \n"
        "jmp 1b \n"

    "1: \n"
        :
        : "m" (b), "m" (len)
    );
#endif

	return res;
}

// Finds and returns the count of bytes needed to represent the number (byte length)
// Starting from the most significant byte, decrementing from N if the byte equals 0
template <unsigned int N>
int BigInt<N>::byteCount() const
{
	int count = N;
	for (char* end = (char*)bytes + N - 1 ; end >= (char*)bytes && !*end; end--, count--);

	return count;
}

// Finds and returns the count of bits needed to represent the number (bit length)
// Starting from the most significant byte, decrementing 8 from the initial count (N-1)*8 if the byte is 0
// Once a byte is found that is non zero, it is right shifted repeatedly till it becomes 0, and the counter is incremented
template <unsigned int N>
int BigInt<N>::bitCount() const
{
	int count = (N-1) << 3;
	char* end = (char*)bytes + N - 1;

	for (; !*end && end > (char*)bytes; end--, count -= 8);
	for (unsigned char nzbyte = *end;  nzbyte; nzbyte >>= 1, count++);

	return count;
}
