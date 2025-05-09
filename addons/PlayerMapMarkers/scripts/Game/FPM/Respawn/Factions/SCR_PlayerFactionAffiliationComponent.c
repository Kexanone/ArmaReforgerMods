//------------------------------------------------------------------------------------------------
modded class SCR_PlayerFactionAffiliationComponent : SCR_FactionAffiliationComponent
{
	protected Faction m_pFPM_ReconnectedFaction;
	
	//------------------------------------------------------------------------------------------------
	void FPM_UpdateReconnectedFaction()
	{
		m_pFPM_ReconnectedFaction = GetAffiliatedFaction();
	}
	
	//------------------------------------------------------------------------------------------------
	Faction FPM_GetReconnectedFaction()
	{
		return m_pFPM_ReconnectedFaction;
	}	
}
