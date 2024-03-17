class KSC_BitTools
{
	//------------------------------------------------------------------------------------------------
	//! Set bit at given position
	static void SetBit(inout int bits, int pos)
	{
		bits |= (1 << pos);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Clear bit at given position
	static void ClearBit(inout int bits, int pos)
	{
		bits &= ~(1 << pos);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Toggle bit at given position
	static void ToggleBit(inout int bits, int pos)
	{
		bits ^= (1 << pos);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if bit at given position is set
	static bool IsBitSet(int bits, int pos)
	{
		return bits & (1 << pos);
	}
}