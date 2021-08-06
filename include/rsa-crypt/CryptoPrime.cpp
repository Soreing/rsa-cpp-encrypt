#define PRIME_LIST_SIZE 100
#define PRIME_LIST_MAX 200

// List of small primes for fast prime checking
static int primesList[] =
{	2,    3,    5,    7,   11,   13,   17,   19,   23,   29,   31,   37,   41,   43,   47,   53,   59,   61,   67,   71,
	179,  181,  191,  193,  197,  199,  211,  223,  227,  229,  233,  239,  241,  251,  257,  263,  269,  271,  277,  281,
	283,  293,  307,  311,  313,  317,  331,  337,  347,  349,  353,  359,  367,  373,  379,  383,  389,  397,  401,  409,
	419,  421,  431,  433,  439,  443,  449,  457,  461,  463,  467,  479,  487,  491,  499,  503,  509,  521,  523,  541,
	547,  557,  563,  569,  571,  577,  587,  593,  599,  601,  607,  613,  617,  619,  631,  641,  643,  647,  653,  659,
	661,  673,  677,  683,  691,  701,  709,  719,  727,  733,  739,  743,  751,  757,  761,  769,  773,  787,  797,  809,
	811,  821,  823,  827,  829,  839,  853,  857,  859,  863,  877,  881,  883,  887,  907,  911,  919,  929,  937,  941,
	947,  953,  967,  971,  977,  983,  991,  997,  1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
	1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223
};

template <unsigned int N>
int shiftCount(const BigInt<N> &prime)
{
	unsigned char* start = (unsigned char*)&prime;
	unsigned char  mask = 1;
	int count = 0;

	while (*start == 0)
	{	count += 8;
		start++;
	}

	while (!(*start & mask))
	{	count++;
		mask <<= 1;
	}

	return count;
}

template <unsigned int N>
bool isPrimeFast(const BigInt<N> &prime)
{
	BigInt<N> small;

	for (int i = 0; i < PRIME_LIST_SIZE; i++)
	{	
		*(int*)&small = primesList[i];
		if ((prime % small).isZero())
		{	return false;
        }
	}

	return true;
}

template <unsigned int N>
bool isPrimeMR(const BigInt<N> &prime, int precision)
{
	MontgomeryDomain<N> domain(prime);
	BigInt<N> m = prime;
	*(unsigned char*)&m &= 254;

	BigInt<N * 2> mOne = domain.transform(m);
	BigInt<N * 2> one = domain.transform(1);
	BigInt<N * 2> a;

	int k = shiftCount(m);
	m >>= k;

	for (int ctr = 0, i = 2; i < precision + 2; i++)
	{
		a = domain.transform(i);
		crypto_pow(a, m, domain);

		if (a != mOne && a != one)
		{
			for (ctr = 0; ctr < k; ctr++)
			{
				domain.square(a);
				if (a == one) return false;
				if (a == mOne) break;
			}
			if (ctr == k) return false;
		}
	}

	return true;
}

template <unsigned int N>
bool isPrime(const BigInt<N> &prime, int precision = 50)
{	return isPrimeFast(prime) && isPrimeMR(prime, precision);
}

template <unsigned int N>
void genratePrime(BigInt<N> &prime, int precision = 50)
{
	prime = rand<N>();

	((char*)&prime)[0]   |= 1;
    ((char*)&prime)[N-1] |= 128;

	for (bool pr = false; !pr; prime += 2)
	{	if (isPrime(prime, precision))
		{	break;
		}
	}
}