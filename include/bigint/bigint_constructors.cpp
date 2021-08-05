
//----------------------------------------------------------------------------//
//                         Constructors and Assignment                        //
//--------------------------------------------------------------------------- //

// Constructs a Big Integer of N bytes from a base 2 big endian string format of the number.
// If the format is invalid, an exception is thrown. If too many digits are present, they are ignored.
// The pointer of the string is moved format if there are more digits than bytes.
// The array of digits are processed one by one and assigned to the object 1 bit at a time
// using |= and << for the correct positions within the byte in little endian format.
template <unsigned int N>
void BigInt<N>::base2(cstr num)
{
	int nlen = strlen(num);
	int len = nlen;
	if (len > N * 8)
		len = N * 8;

	cstr ptr = num + nlen - len;
	memset(bytes, 0, N);

	for (int ch = len - 1, byte = 0, bit = 0; ch >= 0; ch--)
	{
		if ((unsigned char)(ptr[ch] - '0') <= 1)
			bytes[byte] |= (ptr[ch] - '0') << bit;
		else
			throw - 1;

		if (++bit == 8)
		{
			byte++;
			bit = 0;
		}
	}
}

// Constructs a Big Integer of N bytes from a base 8 big endian string format of the number.
// If the format is invalid, an exception is thrown. If too many digits are present, they are ignored.
// The function first identifies the number of padding characters (=) to adjust the location and
// value of the least significant digit. The value of digits are found from a lookup table where
// the ascii value of the character is the index to the values. The 6 bit values are shifter
// to the right position (<<) and added to the bytes (|=) one by one. 
template <unsigned int N>
void BigInt<N>::base8(const cstr num)
{
	int nlen = strlen(num);
	memset(bytes, 0, N);

	short digit;
	int   shift = 0;

	for (int ch = nlen - 1, byte = 0; ch >= 0 && byte<N; ch--)
	{
		digit = num[ch] - '0';
		if ((unsigned short)digit > 7)
			throw - 1;

		digit <<= shift;
		bytes[byte] |= *(char*)&digit;

		if (shift >= 5)
			if (++byte < N)
				bytes[byte] |= *(((char*)&digit) + 1);

		shift += 3;
		if (shift >= 8)
			shift -= 8;
	}
}

// Constructs a Big Integer of N bytes from a base 10 big endian string format of the number, negative or positive.
// The string is copied into a new array and adjusted to contain 0-9 as numbers in each index position.
// If the format is invalid, an exception is thrown.
// The array of numbers are repeatedly divided by 2 using right shift and carry in assembly and the 
// number is constructed bit by bit in little endian format.
// If the format was negative, the result is negated.
template <unsigned int N>
void BigInt<N>::base10(const cstr num)
{
    int len = strlen(num);

	char* copy = new char[len + 1];
	char* cend = copy + len;

    memset(bytes, 0, N);


	for (int i = len; i > 0; i--)
	{   copy[i] = num[i - 1] - '0';
		if (num[i - 1] > '9' || num[i - 1] < '0')
			throw - 1;
	}
	copy[0] = -1;

	unsigned char mask = 1;
	for(int byte = 0; copy[len] !=- 1 && byte<N;)
	{
        if (copy[len] & 1)
            bytes[byte] |= mask;
        copy[len] >>= 1;

        int i = len - 1;
        for (; copy[i] != -1; i--)
        {
            if (copy[i] & 1)
                copy[i + 1] += 5;
            copy[i] >>= 1;
        }

        if (copy[i + 1] == 0)
            copy[i + 1] = -1;

        if (mask == 128)
        {   mask = 1;
            byte++;
        }
        else
            mask <<= 1;
    }

    delete[] copy;
}

// Constructs a Big Integer of N bytes from a base 16 big endian string format of the number.
// If the format is invalid, an exception is thrown. If too many digits are present, they are ignored.
// The pointer of the string is moved format if there are more digits than bytes.
// The array of digits are processed one by one and assigned to the object 4 bits at a time
// using |= and << for the correct positions within the byte in little endian format.
template <unsigned int N>
void BigInt<N>::base16(const cstr num)
{
	int nlen = strlen(num);
	int len = nlen;
	if (len > N * 2)
		len = N * 2;

	cstr ptr = num + nlen - len;
	memset(bytes, 0, N);

	for (int ch = len - 1, byte = 0; ch >= 0; ch--, byte++)
	{
		if ((unsigned char)(ptr[ch] - '0') <= 9)
			bytes[byte] = (ptr[ch] - '0');
		else if (ptr[ch] >= 'A' && ptr[ch] <= 'F')
			bytes[byte] = (ptr[ch] - '7');
		else if (ptr[ch] >= 'a' && ptr[ch] <= 'f')
			bytes[byte] = (ptr[ch] - 'W');
		else
			throw - 1;

		if (--ch < 0)
			break;

		if ((unsigned char)(ptr[ch] - '0') <= 9)
			bytes[byte] |= (ptr[ch] - '0') << 4;
		else if (ptr[ch] >= 'A' && ptr[ch] <= 'F')
			bytes[byte] |= (ptr[ch] - '7') << 4;
		else if (ptr[ch] >= 'a' && ptr[ch] <= 'f')
			bytes[byte] |= (ptr[ch] - 'W') << 4;
		else
			throw - 1;
	}
}

// Constructs a Big Integer of N bytes from a base 64 big endian string format of the number.
// If the format is invalid, an exception is thrown. If too many digits are present, they are ignored.
// The function first identifies the number of padding characters (=) to adjust the location and
// value of the least significant digit. The value of digits are found from a lookup table where
// the ascii value of the character is the index to the values. The 6 bit values are shifter
// to the right position (<<) and added to the bytes (|=) one by one. 
template <unsigned int N>
void BigInt<N>::base64(const cstr num)
{
	int nlen = strlen(num);
	int padding = 0;
	memset(bytes, 0, N);

	if (nlen == 0)
		return;

	if (nlen <3)
		throw - 1;

	if (num[nlen - 1] == '=')
	{	padding++;
		nlen--;
	}

	if (num[nlen - 1] == '=')
	{	padding++;
		nlen--;
	}

	short digit;
	int   shift = 6 - (padding << 1);

	if (v64[num[nlen - 1]] == -1)
		throw - 1;

	bytes[0] = v64[num[nlen - 1]] >> (2 * padding);

	for (int ch = nlen - 2, byte = 0; ch >= 0 && byte<N; ch--)
	{
		digit = v64[(unsigned char)num[ch]];
		if (digit == -1)
			throw - 1;

		digit <<= shift;
		bytes[byte] |= *(char*)&digit;

		if (shift != 0)
			if (++byte < N)
				bytes[byte] |= *(((char*)&digit) + 1);

		shift += 6;
		if (shift >= 8)
			shift -= 8;
	}
}

// Constructs a Big Integer of N bytes, all set to 0.
template <unsigned int N>
BigInt<N>::BigInt()
{
	memset(bytes, 0, N);
}

// Constructs a Big Integer of N bytes using the value of another Big Integer of the same size
// The bytes of the other Big Integer are copied into the array of this object.
template <unsigned int N>
BigInt<N>::BigInt(const BigInt<N> &other)
{
	memcpy(bytes, other.bytes, N);
}


template <unsigned int N>
BigInt<N>::BigInt(const void* other)
{
	memcpy(bytes, other, N);
}

// Constructs a Big Integer of N bytes using the value of a primitive integer type.
// The bytes of the Big Integer are set to 0 of 255 based on the number's sign,
// then the integer is copied to the beginning.
template <unsigned int N>
BigInt<N>::BigInt(const unsigned long long val)
{
	memset(bytes, val < 0 ? 255 : 0, N);
	*(unsigned long long*)bytes = val;
}

// Constructs a Big Integer of N bytes using a byte array representing an integer
// The byte array is copied into the structure, then the rest of the space is filled with 0s
// If the data represented a bigEndian data, the bytes are reversed
template <unsigned int N>
BigInt<N>::BigInt(const cstr data, int len, int bigEndian)
{
	memcpy(bytes, data, len);
	memset(bytes+len, 0, N-len);

	if(bigEndian)
	{	unsigned char temp;
		for(int i=0; i<len/2; i++)
		{	temp = bytes[i];
			bytes[i] = bytes[len-i-1];
			bytes[len-i-1] = temp;
		}
	}
}


// Constructs a Big Integer of N bytes using the value of another Big Integer of the same size
// The bytes of the other Big Integer are copied into the array of this object.
template <unsigned int N>
BigInt<N>& BigInt<N>::operator=(const BigInt<N> &other)
{
	memcpy(bytes, other.bytes, N);
	return *this;
}

// Constructs a Big Integer of N bytes using a string format of the number and a base.
// The constructor accepts formats in different bases, 2, 8, 10, 16, 64.
// If the format or the base is invalid, an exception is thrown.
template <unsigned int N>
BigInt<N>::BigInt(const cstr num, const int base)
{
	int prefix = 0;
	int len = strlen(num);
	if (len > 0 && num[0] == '0')
	{
		if (len > 1 && (num[1] == 'x' || num[1] == '#' || num[1] == 'b'))
			prefix++;
		prefix++;
	}

	try
	{
		switch (base)
		{
		case 2:  base2(num + prefix); break;
		case 8:  base8(num + prefix); break;
		case 10: base10(num + prefix); break;
		case 16: base16(num + prefix); break;
		case 64: base64(num + prefix); break;
		default: throw - 1;
		}
	}
	catch (int)
	{
		throw - 1;
	}
}

// Constructs a Big Integer of N bytes using a string format of the number.
// The constructor accepts formats in different bases, 2, 8, 10, 16, 64.
// If 0b (for binary), 0 (for octal), 0x (for hexadecimal) or 0# (for base64) 
// is found in the beginning, the appropriate base is used for the construction,
// otherwise base 10 is the default base for interpreting the numerical string.
// If the format is invalid, an exception is thrown.
template <unsigned int N>
BigInt<N>::BigInt(const cstr num)
{
	int len = strlen(num);

	try
	{
		if (len > 1 && num[0] == '0' && (num[1] == 'b' || num[1] == 'B'))
			base2(num + 2);
		else if (len > 1 && num[0] == '0' && (num[1] == 'x' || num[1] == 'X'))
			base16(num + 2);
		else if (len > 1 && num[0] == '0' && num[1] == '#')
			base64(num + 2);
		else if (len > 0 && num[0] == '0')
			base8(num + 1);
		else
			base10(num);
	}
	catch (int)
	{
		throw - 1;
	}
}