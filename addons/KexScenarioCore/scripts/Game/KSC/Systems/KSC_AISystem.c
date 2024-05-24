//------------------------------------------------------------------------------------------------
class KSC_AISystem : GameSystem
{
	protected ref array<KSC_AO> m_aManagedAOs;
	
	//------------------------------------------------------------------------------------------------
	void Register(KSC_AO ao)
	{
		m_aManagedAOs.Insert(ao);
	}
	
	//------------------------------------------------------------------------------------------------
	void Unregister(KSC_AO ao)
	{
		m_aManagedAOs.RemoveItem(ao);
	}
}
