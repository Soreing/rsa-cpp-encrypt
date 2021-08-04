
//----------------------------------------------------------------------------//
//                             Relational Operators                           //
//--------------------------------------------------------------------------- //


template <unsigned int N>
bool BigInt<N>::isZero() const
{
	const unsigned int* begp = (const unsigned int*)(bytes);
	const unsigned int* endp = (const unsigned int*)(bytes + N);
	bool res = true;

#if defined(_MSVC_INTEL) && defined(_X64)
	__asm
	{
		mov rsi, begp;
		mov rdx, endp;

	next:
		cmp rsi, rdx;
		jge end;

		lodsd;
		cmp eax, 0;
		jne fl;

		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
    __asm
    {
        mov esi, begp;
        mov edx, endp;

    next:
        cmp esi, edx;
        jge end;

        lodsd;
        cmp eax, 0;
        jne fl;

        jmp next;

    fl:
        mov res, 0;
    end:
    }
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
    (
        "mov %1, %%rsi \n"
        "mov %2, %%rdx \n"

    "1: \n"
        "cmp %%rdx, %%rsi \n"
        "jge 2f \n"

        "lodsl \n"
        "cmp $0, %%eax \n"
        "jne 1f \n"

        "jmp 1b \n"

    "1: \n"
        "movb $0, %0 \n"
    "2: \n"
        : "=m" (res)
        : "m" (begp), "m" (endp)
    );
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
        "mov %1, %%esi \n"
        "mov %2, %%edx \n"

    "1: \n"
        "cmp %%edx, %%esi \n"
        "jge 2f \n"

        "lodsl \n"
        "cmp $0, %%eax \n"
        "jne 1f \n"

        "jmp 1b \n"

    "1: \n"
        "movb $0, %0 \n"
    "2: \n"
        : "=m" (res)
        : "m" (begp), "m" (endp)
    );
#endif

	return res;
}

// Compares 2 Big Integers of the same size, returns true if the left side is larger.
// Uses the implementation of the <= operator and negates the result.
template <unsigned int N>
bool BigInt<N>::operator>(const BigInt<N> &right) const
{
	return !(*this <= right);
}

// Compares 2 Big Integers of the same size, returns true if the left side is smaller.
// Uses the implementation of the >= operator and negates the result.
template <unsigned int N>
bool BigInt<N>::operator<(const BigInt<N> &right) const
{
	return !(*this >= right);
}

// Compares 2 Big Integers of the same size, returns true if they are not equal.
// Uses the implementation of the == operator and negates the result.
template <unsigned int N>
bool BigInt<N>::operator!=(const BigInt<N> &right) const
{
	return !(*this == right);
}

// Compares 2 Big Integers of the same size by comparing the bytes of the numbers in Assembly 
// from the largest to the smallest bytes (left to right in memory in Little Endian order).
// If either number is negative, a result is returned immediately.
// Iterates through the memory 4 bytes at a time and returns false immediately when 4 bytes
// of the left hand operand is less than the right hand.
template <unsigned int N>
bool BigInt<N>::operator>=(const BigInt<N> &right) const
{
	const unsigned int* r = (const unsigned int*)(right.bytes + N - 4);
	const unsigned int* l = (const unsigned int*)(bytes + N - 4);

	int len = (N >> 2);
	bool res = 1;

#if defined(_MSVC_INTEL) && defined(_X64)
	__asm
	{
		mov rbx, r;
		mov rdx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [rdx];
		cmp eax, [rbx];

		jb fl;
		ja end;

		sub rdx, 4;
		sub rbx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov ebx, r;
		mov edx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [edx];
		cmp eax, [ebx];

		jb fl;
		ja end;

		sub edx, 4;
		sub ebx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %1, %%rsi \n"
		"mov %2, %%rdi \n"
		"mov %3, %%ecx \n"

	"1: \n"
		"jecxz 2f \n"
		"dec %%ecx \n"

		"mov (%%rdi), %%eax \n"
		"cmp (%%rsi), %%eax \n"

		"jb 1f \n"
		"ja 2f \n"

		"sub $4, %%rdi \n"
		"sub $4, %%rsi \n"
		"jmp 1b \n"

	"1: \n"
		"movb $0, %0 \n"
	"2: \n"
        : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
	);
#elif defined(_GAS_ATT) && defined(_X32)
    __asm__
    (
        "mov %1, %%esi \n"
        "mov %2, %%edi \n"
        "mov %3, %%ecx \n"

    "1: \n"
        "jecxz 2f \n"
        "dec %%ecx \n"

        "mov (%%edi), %%eax \n"
        "cmp (%%esi), %%eax \n"

        "jb 1f \n"
        "ja 2f \n"

        "sub $4, %%edi \n"
        "sub $4, %%esi \n"
        "jmp 1b \n"

    "1: \n"
        "movb $0, %0 \n"
    "2: \n"
        : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
    );
#endif

	return res;
}

// Compares 2 Big Integers of the same size by comparing the bytes of the numbers in Assembly 
// from the largest to the smallest bytes (left to right in memory in Little Endian order).
// If either number is negative, a result is returned immediately.
// Iterates through the memory 4 bytes at a time and returns false immediately when 4 bytes
// of the left hand operand is greater than the right hand.
template <unsigned int N>
bool BigInt<N>::operator<=(const BigInt<N> &right) const
{
	const unsigned int* r = (const unsigned int*)(right.bytes + N - 4);
	const unsigned int* l = (const unsigned int*)(bytes + N - 4);

	int len = (N >> 2);
	bool res = 1;

#if defined(_MSVC_INTEL) && defined(_X64)
	__asm
	{
		mov rbx, r;
		mov rdx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [rdx];
		cmp eax, [rbx];

		ja fl;
		jb end;

		sub rdx, 4;
		sub rbx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov ebx, r;
		mov edx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [edx];
		cmp eax, [ebx];

		ja fl;
		jb end;

		sub edx, 4;
		sub ebx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %1, %%rsi \n"
		"mov %2, %%rdi \n"
		"mov %3, %%ecx \n"

	"1: \n"
		"jecxz 2f \n"
		"dec %%ecx \n"

		"mov (%%rdi), %%eax \n"
		"cmp (%%rsi), %%eax \n"

		"ja 1f \n"
		"jb 2f \n"

		"sub $4, %%rdi \n"
		"sub $4, %%rsi \n"
		"jmp 1b \n"

	"1: \n"
		"movb $0, %0 \n"
	"2: \n"
	    : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
	);
#elif defined(_GAS_ATT) && defined(_X32)
	__asm__
	(
		"mov %1, %%esi \n"
		"mov %2, %%edi \n"
		"mov %3, %%ecx \n"

	"1: \n"
		"jecxz 2f \n"
		"dec %%ecx \n"

		"mov (%%edi), %%eax \n"
		"cmp (%%esi), %%eax \n"

		"ja 1f \n"
		"jb 2f \n"

		"sub $4, %%edi \n"
		"sub $4, %%esi \n"
		"jmp 1b \n"

	"1: \n"
		"movb $0, %0 \n"
	"2: \n"
	    : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
	);
#endif

	return res;
}

// Compares 2 Big Integers of the same size by comparing the bytes of the numbers in Assembly 
// from the largest to the smallest bytes (left to right in memory in Little Endian order).
// Iterates through the memory 4 bytes at a time and returns false immediately when 4 bytes
// of the numbers do not match.
template <unsigned int N>
bool BigInt<N>::operator==(const BigInt<N> &right) const
{
	const unsigned int* r = (const unsigned int*)(right.bytes + N - 4);
	const unsigned int* l = (const unsigned int*)(bytes + N - 4);
	int len = (N >> 2);
	bool res = 1;
	int ctr = 0;

#if defined(_MSVC_INTEL) && defined(_X64)
	__asm
	{
		mov rbx, r;
		mov rdx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [rdx];
		cmp eax, [rbx];

		jne fl;

		sub rdx, 4;
		sub rbx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_MSVC_INTEL) && defined(_X32)
	__asm
	{
		mov ebx, r;
		mov edx, l;
		mov ecx, len;

	next:
		jecxz end;
		dec ecx;

		mov eax, [edx];
		cmp eax, [ebx];

		jne fl;

		sub edx, 4;
		sub ebx, 4;
		jmp next;

	fl:
		mov res, 0;
	end:
	}
#elif defined(_GAS_ATT) && defined(_X64)
    __asm__
	(
		"mov %1, %%rsi \n"
		"mov %2, %%rdi \n"
		"mov %3, %%ecx \n"

	"1: \n"
		"jecxz 2f \n"
		"dec %%ecx \n"

		"mov (%%rdi), %%eax \n"
		"cmp (%%rsi), %%eax \n"

		"jne 1f \n"

		"sub $4, %%rdi \n"
		"sub $4, %%rsi \n"
		"jmp 1b \n"

	"1: \n"
		"movb $0, %0 \n"
	"2: \n"
	    : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
	);
#elif defined(_GAS_ATT) && defined(_X32)
	__asm__
	(
		"mov %1, %%esi \n"
		"mov %2, %%edi \n"
		"mov %3, %%ecx \n"

	"1: \n"
		"jecxz 2f \n"
		"dec %%ecx \n"

		"mov (%%edi), %%eax \n"
		"cmp (%%esi), %%eax \n"

		"jne 1f \n"

		"sub $4, %%edi \n"
		"sub $4, %%esi \n"
		"jmp 1b \n"

	"1: \n"
		"movb $0, %0 \n"
	"2: \n"
	    : "=m" (res)
        : "m" (r), "m" (l), "m" (len)
	);
#endif

	return res;
}

// Swaps the operands for > when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator>(const T left, const BigInt<N> &right)
{
	return right < left;
}

// Swaps the operands for < when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator<(const T left, const BigInt<N> &right)
{
	return right > left;
}

// Swaps the operands for == when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator==(const T left, const BigInt<N> &right)
{
	return right == left;
}

// Swaps the operands for >= when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator>=(const T left, const BigInt<N> &right)
{
	return right <= left;
}

// Swaps the operands for <= when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator<=(const T left, const BigInt<N> &right)
{
	return right >= left;
}

// Swaps the operands for != when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator!=(const T left, const BigInt<N> &right)
{
	return right != left;
}