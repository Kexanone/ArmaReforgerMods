//------------------------------------------------------------------------------------------------
class KSC_Math
{
	//------------------------------------------------------------------------------------------------
	//! Same as Math::RandomInt, but supports ranges up to 30 bits wide
	static int RandomInt(int min, int max)
	{
		int diff = max - min;
		int nHighBits = Math.Ceil(Math.Log2(diff)) - 15;
		if (nHighBits <= 0)
			return Math.RandomInt(0, diff) + min;
		
		int value = diff;
		while (value >= diff)
		{
			value = Math.RandomInt(0, 1 << 15);
			value |= Math.RandomInt(0, 1 << nHighBits) << 15;
		}
		
		return value + min;
	}
}
