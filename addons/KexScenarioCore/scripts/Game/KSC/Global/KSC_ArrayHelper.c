//------------------------------------------------------------------------------------------------
//! Same as SCR_ArrayHelperT, but for types T that do not support strong references
class KSC_ArrayHelperT<Class T>
{
	//------------------------------------------------------------------------------------------------
	static void Shuffle(notnull inout array<T> items, int shuffles = 1)
	{
		if (items.Count() < 2)
			return;
		
		// two checks are faster than Math.ClampInt
		if (shuffles < 1)
			shuffles = 1;

		if (shuffles > 10)
			shuffles = 10;
		
		Math.Randomize(-1);

		while (shuffles > 0)
		{
			for (int i = 0, count = items.Count(); i < count; i++)
			{
				int index1 = Math.RandomInt(0, count);
				int index2 = Math.RandomInt(0, count);
				if (index1 != index2)
					items.SwapItems(index1, index2);
			}

			shuffles--;
		}
	}
}
