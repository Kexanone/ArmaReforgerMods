//------------------------------------------------------------------------------------------------
//! This array behaves like an urn problem
class KSC_UrnArray<Class T> : Managed
{
	protected ref array<T> m_aElements = {};
	protected bool m_bShuffled = false;
	
	void Insert(T value)
	{
		m_aElements.Insert(value);
		m_bShuffled = false;
	}
	
	void InsertAll(array<T> values)
	{
		m_aElements.InsertAll(values);
		m_bShuffled = false;
	}
	
	int Count()
	{
		return m_aElements.Count();
	}
	
	T GetRandomElement()
	{
		if (m_aElements.IsEmpty())
			return null;
		
		if (!m_bShuffled)
		{
			KSC_ArrayHelperT<T>.Shuffle(m_aElements);
			m_bShuffled = true;
		}
		
		T element = m_aElements.Get(0);
		m_aElements.Remove(0);
		return element;
	}
}
