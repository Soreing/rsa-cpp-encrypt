
template <unsigned int N>
MontgomeryDomain<N>::MontgomeryDomain(const BigInt<N> &m)
{
	shift = m.bitCount();

	memcpy(&mod, &m, N);
	r = BigInt<N * 2>(1) << shift;
	R = crypto_inverse(r - mod, r);

	mask = r;
	mask--;

	fast = (shift == (N * 8) ? true : false);
}

template <unsigned int N>
BigInt<N * 2> MontgomeryDomain<N>::slow_transform(const BigInt<N> &val)
{
	BigInt<N * 2> temp;

	memcpy(&temp, &val, N);
	temp <<= shift;
	return temp % mod;
}

template <unsigned int N>
BigInt<N * 2> MontgomeryDomain<N>::fast_transform(const BigInt<N> &val)
{
	BigInt<N * 2> temp;

	memcpy((char*)&temp + N, &val, N);
	memset(&temp, 0, N);
	return temp % mod;
}

template <unsigned int N>
BigInt<N * 2> MontgomeryDomain<N>::transform(const BigInt<N> &val)
{	return fast ? fast_transform(val) : slow_transform(val); 
}

template <unsigned int N>
BigInt<N> MontgomeryDomain<N>::slow_revert(const BigInt<N * 2> &val)
{
	return BigInt<N>((void*)&multiply(BigInt<N*2>(val), BigInt<N * 2>(1)));
}

template <unsigned int N>
BigInt<N> MontgomeryDomain<N>::fast_revert(const BigInt<N * 2> &val)
{
	return BigInt<N>((void*)&fast_multiply(BigInt<N*2>(val), BigInt<N * 2>(1)));
}

template <unsigned int N>
BigInt<N> MontgomeryDomain<N>::revert(const BigInt<N * 2> &val)
{	
	return fast ? fast_revert(val) : slow_revert(val);
}

template <unsigned int N>
BigInt<N * 2>& MontgomeryDomain<N>::slow_square(BigInt<N * 2> &num)
{
	char* x[N * 2], n[N * 2];
	bool overflow = false;

	//x = num * num;
	::square<N>((char*)&num);
	memcpy(x, &num, N * 2);


	//s = ((x & mask)*R) & mask;
	num &= mask;
	karatsuba<N>((char*)&num, (char*)&R);
	num &= mask;

	//t = (x + s*mod) >> shift;
	karatsuba<N>((char*)&num, (char*)&mod);
	memcpy(n, (char*)&num, N * 2);
	num += *(BigInt<N * 2>*)x;

	if (num < *(BigInt<N * 2>*)x && num < *(BigInt<N * 2>*)n)
	{	overflow = true;
	}

	num >>= shift;

	if (overflow)
	{	*((char*)&num + shift/8) |= 1<< (shift&7);
	}

	if (num >= mod)
	{	num -= mod;
	}

	return num;
}

template <unsigned int N>
BigInt<N * 2>& MontgomeryDomain<N>::fast_square(BigInt<N * 2> &num)
{
	char* x[N * 2], n[N * 2];
	bool overflow = false;

	//x = num * num;
	::square<N>((char*)&num);
	memcpy(x, &num, N * 2);

	//s = ((x & mask)*R) & mask;
	memset((char*)&num + N, 0, N);
	karatsuba<N>((char*)&num, (char*)&R);
	memset((char*)&num + N, 0, N);

	//t = (x + s*mod) >> shift;
	karatsuba<N>((char*)&num, (char*)&mod);
	memcpy(n, (char*)&num, N * 2);
	num += *(BigInt<N * 2>*)x;

	if (num < *(BigInt<N * 2>*)x && num < *(BigInt<N * 2>*)n)
	{	overflow = true;
	}

	memcpy(&num, (char*)&num + N, N);
	memset((char*)&num + N, 0, N);

	if (overflow)
	{	*((char*)&num + N) = 1;
	}

	if (num >= mod)
	{	num -= mod;
	}

	return num;
}

template <unsigned int N>
BigInt<N * 2> MontgomeryDomain<N>::square(BigInt<N * 2> &num)
{	return fast ? fast_square(num) : slow_square(num);
}

template <unsigned int N>
BigInt<N * 2>& MontgomeryDomain<N>::slow_multiply(BigInt<N * 2> &left, BigInt<N * 2> &right)
{
	char* x[N * 2];

	//x = left * right;
	karatsuba<N>((char*)&left, (char*)&right);
	memcpy(x, &left, N * 2);

	//s = ((x & mask)*R) & mask;
	left &= mask;
	karatsuba<N>((char*)&left, (char*)&R);
	left &= mask;

	//t = (x + s*mod) >> shift;
	karatsuba<N>((char*)&left, (char*)&mod);
	left += *(BigInt<N * 2>*)x;

	left >>= shift;

	if (left >= mod)
	{	left -= mod;
	}

	return left;
}

template <unsigned int N>
BigInt<N * 2>& MontgomeryDomain<N>::fast_multiply(BigInt<N * 2> &left, BigInt<N * 2> &right)
{
	char* x[N * 2], n[N * 2];
	bool overflow = false;

	//x = left * right;
	karatsuba<N>((char*)&left, (char*)&right);
	memcpy(x, &left, N * 2);

	//s = ((x & mask)*R) & mask;
	memset((char*)&left + N, 0, N);
	karatsuba<N>((char*)&left, (char*)&R);
	memset((char*)&left + N, 0, N);

	//t = (x + s*mod) >> shift;
	karatsuba<N>((char*)&left, (char*)&mod);
	memcpy(n, (char*)&left, N * 2);
	left += *(BigInt<N * 2>*)x;

	if (left < *(BigInt<N * 2>*)x && left < *(BigInt<N * 2>*)n)
	{	overflow = true;
	}

	memcpy(&left, (char*)&left + N, N);
	memset((char*)&left + N, 0, N);

	if (overflow)
	{	*((char*)&left + N) = 1;
	}

	if (left >= mod)
	{	left -= mod;
	}

	return left;
}

template <unsigned int N>
BigInt<N * 2> MontgomeryDomain<N>::multiply(BigInt<N * 2> &left, BigInt<N * 2> &right)
{	return fast ? fast_multiply(left, right) : slow_multiply(left, right);
}

template <unsigned int N>
BigInt<N> crypto_gcd(BigInt<N> a, BigInt<N> b)
{
	BigInt<N> temp;

	if (a > b)
	{
		temp = a;
		a = b;
		b = temp;
	}

	while (true)
	{
		BigInt<N>::div(b, a, &b, &temp);

		if (temp.isZero())
		{	return a;
		}

		b = a;
		a = temp;

	}
}

template <unsigned int N>
BigInt<N> crypto_inverse(BigInt<N> x, BigInt<N> mod)
{
	int ctr = 0;
	BigInt<N> p2;
	BigInt<N> p1 = 1;
	BigInt<N> p0 = 1;

	BigInt<N> temp;
	BigInt<N> original = mod;

	while (true)
	{
		BigInt<N>::div(mod, x, &mod, &temp);

		if (temp.isZero())
		{
			if (x != "1")
			{	return temp;
			} 
			else if (ctr & 1)
			{	return original - p0;
			}
			else
			{	return p0;
			}
		}

		p0 = p0 * mod + p2;
		p2 = p1;
		p1 = p0;

		mod = x;
		x = temp;
		ctr++;
	}
}

template <unsigned int N>
void crypto_pow(BigInt<N * 2> &x, BigInt<N> &exp, MontgomeryDomain<N> &dom)
{
	BigInt<N * 2> num = x;
	int len = exp.bitCount() - 1;
	unsigned char* end = (unsigned char*)&exp + len / 8;
	unsigned char* beg = (unsigned char*)&exp;
	unsigned char  mask = 1 << ((len - 1) & 7);

	if (mask == 128)
		end--;

	while (end >= beg)
	{
		dom.square(x);

		if (*end & mask)
		{	dom.multiply(x, num);
		}
		mask >>= 1;
		if (!mask)
		{	mask = 128;
			end--;
		}
	}
}