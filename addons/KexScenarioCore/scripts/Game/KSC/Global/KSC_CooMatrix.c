//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class KSC_CooMatrix<Class T> : Managed
{
	[Attribute()]
	protected ref array<int> m_aRowIdcs;
	
	[Attribute()]
	protected ref array<int> m_aColIdcs;
	
	[Attribute()]
	protected ref array<T> m_Data;
	
	//------------------------------------------------------------------------------------------------
	array<int> GetRowIcs()
	{
		return m_aRowIdcs;
	}
	
	//------------------------------------------------------------------------------------------------
	array<int> GetColIcs()
	{
		return m_aColIdcs;
	}
	
	array<T> GetData()
	{
		return m_Data;
	}
	
}
