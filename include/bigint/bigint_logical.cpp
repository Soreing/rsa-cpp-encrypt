
//----------------------------------------------------------------------------//
//                               Logical Operators                            //
//--------------------------------------------------------------------------- //

// Type Casts the Big Integer to a boolean and &&s it with the right hand side.
template <unsigned int N>
bool BigInt<N>::operator&&(const bool right) const
{
	return (bool)*this && right;
}

// Type Casts the Big Integer to a boolean and ||s it with the right hand side.
template <unsigned int N>
bool BigInt<N>::operator||(const bool right) const
{
	return (bool)*this || right;
}

// Type Casts the Big Integer to a boolean and flips the result with !
template <unsigned int N>
bool  BigInt<N>::operator!() const
{
	return !(bool)*this;
}

// Swaps the operands for && when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator&&(const bool left, const BigInt<N> &right)
{
	return left && (bool)right;
}

// Swaps the operands for || when the left hand operand is not a Big Integer.
template <class T, unsigned int N>
bool operator||(const bool left, const BigInt<N> &right)
{
	return left || (bool)right;
}