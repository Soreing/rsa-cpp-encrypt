

template <unsigned int N>
BigInt<N> pow(const BigInt<N> base, const BigInt<N> exp)
{
	BigInt<N> res = 1;
	BigInt<N> num = base;

	int byte = exp.length() - 1;
	int mask = 128;

	while (byte >= 0)
	{	while (mask > 0)
		{	res *= res;
			if (((char*)&exp)[byte] & mask)
			{	res *= num;
			}
			mask >>= 1;
		}

		mask = 128;
		byte--;
	}

	return res;
}

template <unsigned int N>
BigInt<N> rand()
{
	BigInt<N> res;
	for (unsigned char *start = (unsigned char*)res.bytes, *end = (unsigned char*)(res.bytes + N); start != end; start++)
	{	*start = rand();
	}
	//*(res.bytes + N - 1) &= 127;
	return res;
}