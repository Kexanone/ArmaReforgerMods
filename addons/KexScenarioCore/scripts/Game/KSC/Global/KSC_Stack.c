//------------------------------------------------------------------------------------------------
//! Use KSC_RefStack instead if you need strong references to T
class KSC_Stack<Class T> : Managed
{
	protected ref array<T> m_aStack = {};
	
	//------------------------------------------------------------------------------------------------
	void Push(T item)
	{
		m_aStack.Insert(item);
	}
	
	//------------------------------------------------------------------------------------------------
	void PushAll(array<T> items)
	{
		m_aStack.InsertAll(items);
	}
	
	//------------------------------------------------------------------------------------------------
	T Pop()
	{
		int topIdx = Count() - 1;
		T item = m_aStack[topIdx];
		m_aStack.RemoveOrdered(topIdx);
		return item;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsEmpty()
	{
		return m_aStack.IsEmpty();
	}
	
	//------------------------------------------------------------------------------------------------
	int Count()
	{
		return m_aStack.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	void Debug()
	{
		PrintFormat( "KSC_Stack count: %1", Count());

		for (int i = 0; i < Count(); i++)
		{
			PrintFormat("[%1] => %2", i, string.ToString(m_aStack[i]));
		}
	}
}

//------------------------------------------------------------------------------------------------
//! Same as KSC_Stack, but has strong references to T
class KSC_RefStack<Class T> : Managed
{
	protected ref array<ref T> m_aStack = {};
	
	//------------------------------------------------------------------------------------------------
	void Push(T item)
	{
		m_aStack.Insert(item);
	}
	
	//------------------------------------------------------------------------------------------------
	void PushAll(array<T> items)
	{
		m_aStack.InsertAll(items);
	}
	
	//------------------------------------------------------------------------------------------------
	T Pop()
	{
		int topIdx = Count() - 1;
		T item = m_aStack[topIdx];
		m_aStack.RemoveOrdered(topIdx);
		return item;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsEmpty()
	{
		return m_aStack.IsEmpty();
	}
	
	//------------------------------------------------------------------------------------------------
	int Count()
	{
		return m_aStack.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	void Debug()
	{
		PrintFormat( "KSC_Stack count: %1", Count());

		for (int i = 0; i < Count(); i++)
		{
			PrintFormat("[%1] => %2", i, string.ToString(m_aStack[i]));
		}
	}
}