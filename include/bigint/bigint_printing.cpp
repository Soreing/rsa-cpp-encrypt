
//----------------------------------------------------------------------------//
//                          Stream and String Operators                       //
//--------------------------------------------------------------------------- //

// Finds and returns the number of bytes a Big Integer of N bytes occupies
// Without leading 0 bytes. Used for printing.
template <unsigned int N>
int BigInt<N>::length() const
{
	int zeros = 0;
	for (zeros = 0; zeros < N && bytes[N - zeros - 1] == 0; zeros++);

	return N - zeros;
}


// Constructs a string of big endian order binary digits from the value of
// an N byte long Big Integer. The result ignores leading 0s.
// An array of digits is initialized to '0', then for all valid bytes,
// each bit is extracted using & and shift, then added to the '0'.
// Leading zeros are removed from the string, and if all digits were removed,
// "0" is returned as a special case.
template <unsigned int N>
str BigInt<N>::str2() const
{
	int   digitCtr = 0;
	char  digitRaw[N * 8 + 1];
	char* digitStart = digitRaw;
	memset(digitRaw, '0', N * 8);

	for (int i = length() - 1; i >= 0; i--)
	{
		digitRaw[digitCtr++] += (bytes[i] & 128) >> 7;
		digitRaw[digitCtr++] += (bytes[i] & 64) >> 6;
		digitRaw[digitCtr++] += (bytes[i] & 32) >> 5;
		digitRaw[digitCtr++] += (bytes[i] & 16) >> 4;
		digitRaw[digitCtr++] += (bytes[i] & 8) >> 3;
		digitRaw[digitCtr++] += (bytes[i] & 4) >> 2;
		digitRaw[digitCtr++] += (bytes[i] & 2) >> 1;
		digitRaw[digitCtr++] += (bytes[i] & 1);
	}

	digitRaw[digitCtr] = 0;
	while (digitStart[0] == '0')
		digitStart++;

	if (digitStart[0] == 0)
		return "0";
	else
		return str(digitStart);
}

//STUB
template <unsigned int N>
str BigInt<N>::str8() const
{
	char  digits[N * 4 + 1];
	char* digitEnd = digits + N * 4 - 1;
	short digit = 0;
	int   shift = 0;
	int   len = length();

	for (int byte = 0; byte < len; digitEnd--)
	{

		*(char*)&digit = bytes[byte];
		if (shift >= 5)
			if (++byte < len)
				*(((char*)&digit) + 1) = bytes[byte];

		*digitEnd = ((digit >> shift) & 7) + '0';

		shift += 3;
		if (shift >= 8)
			shift -= 8;
	}

	digitEnd++;
	digits[N * 4] = 0;
	while (digitEnd[0] == '0')
		digitEnd++;

	if (digitEnd[0] == 0)
		return "0";
	else
		return str(digitEnd);
}

// Constructs a string of big endian order decimal digits from the value of
// an N byte long Big Integer. The result ignores leading 0s.
// To make sure the digits fit into the c-string, an 4N byte array is declared.
// The sign of the Big Integer is evaluated, and if negative, the value is negated.
// The Big Integer is processed bit by bit, and in each iteration, the array of
// decimal digits is doubled digit by digit left to right, and if over 10, 
// 1 is added to the previous digit, while 10 is subtracted from the current digit.
// A Digit Counter is maintained to know how many digits does the decimal number take up.
// After the entire array was multiplied by 2, the bit is added to the smallest digit.
// The byte mask that handles the bit extraction from bytes is also maintained.
// After all bits were processed, the array is incremented by '0' for ASCII digits.
// If the number was originally negative, the - sign is appended at the start.
template <unsigned int N>
str BigInt<N>::str10() const
{
	int   digitCtr = 1;
	char  digitRaw[N * 4 + 1];
	char* digitEnd = digitRaw + N * 4;
	memset(digitRaw, 0, N * 4 + 1);

	unsigned char copy[N];
	memcpy(copy, bytes, N);

	//For each byte and all bits in the bytes
	unsigned char mask = 128;
	for (int i = length() - 1; i >= 0; )
	{
		for (int j = digitCtr; j > 0; j--)
		{
			//Multiply number by 2 and handle the carry
			digitEnd[-j] += digitEnd[-j];
			if (digitEnd[-j] >= 10)
			{
				digitEnd[-j] -= 10;
				digitEnd[-j - 1]++;

				//Maintain Digit Counter if largest number had overflow
				if (j == digitCtr)
					digitCtr++;
			}
		}

		// Add the bit to the number
		if ((copy[i] & mask) == mask)
			digitEnd[-1]++;

		// (Shift) || (Reset the mask && Move to the next byte)
		if (mask == 1)
		{	mask = 128;
			i--;
		}
		else
			mask >>= 1;
	}

	// Make digits
	for (int i = digitCtr; i>0; i--)
		digitEnd[-i] += '0';

	return str(digitEnd - digitCtr);
}

// Constructs a string of big endian order hexadecimal digits from the value of
// an N byte long Big Integer. The result ignores leading 0s.
// The digits are looked up in an array of characters where the numeric value
// is the index to the corresponding digit. 4 bits are processed at once using & and >>.
// Leading zeros are removed from the string, and if all digits were removed,
// "0" is returned as a special case.
template <unsigned int N>
str BigInt<N>::str16() const
{
	int   digitCtr = 0;
	char  digitRaw[N * 2 + 1];
	char* digitStart = digitRaw;

	for (int i = length() - 1; i >= 0; i--)
	{
		digitRaw[digitCtr++] = b16[(bytes[i] & 0xF0) >> 4];
		digitRaw[digitCtr++] = b16[(bytes[i] & 0x0F)];
	}

	digitRaw[digitCtr] = 0;
	while (digitStart[0] == '0')
		digitStart++;

	if (digitStart[0] == 0)
		return "0";
	else
		return str(digitStart);
}

//STUB
template <unsigned int N>
str BigInt<N>::str64() const
{
	char  digitRaw[(N / 3 + 1) * 4 + 1];
	char* digitStart = digitRaw;
	int len = length();

	long num;

	for (int i = len - 3; i >= 0; i -= 3, digitStart += 4)
	{
		((char*)&num)[0] = bytes[i];
		((char*)&num)[1] = bytes[i + 1];
		((char*)&num)[2] = bytes[i + 2];

		digitStart[3] = b64[num & 63];
		num >>= 6;
		digitStart[2] = b64[num & 63];
		num >>= 6;
		digitStart[1] = b64[num & 63];
		num >>= 6;
		digitStart[0] = b64[num & 63];
	}

	if (len % 3 == 1)
	{
		char leftover = *bytes;
		digitStart[3] = '=';
		digitStart[2] = '=';
		digitStart[1] = b64[(leftover & 3) << 4];
		digitStart[0] = b64[(leftover & 252) >> 2];
	}

	if (len % 3 == 2)
	{
		short leftover = *(short*)bytes;
		digitStart[3] = '=';
		digitStart[2] = b64[(leftover & 15) << 2];
		digitStart[1] = b64[(leftover & 1008) >> 4];
		digitStart[0] = b64[(leftover & 64512) >> 10];
	}

	digitRaw[(len / 3 + 1) * 4] = 0;
	return str(digitRaw);
}

// Returns a string of big endian order digits of a Big Integer of N bytes.
// The representation depends on the base selected (2, 8, 10, 16, 64).
// The base prefix can be optionally appended to the digits.
// If an invalid base is selected, nothing is returned.
template <unsigned int N>
str BigInt<N>::toString(int base, bool prefix) const
{
	switch (base)
	{
	case 2:  return (prefix ? "0b" : "") + str2();
	case 8:  return (prefix ? "0" : "") + str8();
	case 10: return str10();
	case 16: return (prefix ? "0x" : "") + str16();
	case 64: return (prefix ? "0#" : "") + str64();
	default: return "";
	}
}