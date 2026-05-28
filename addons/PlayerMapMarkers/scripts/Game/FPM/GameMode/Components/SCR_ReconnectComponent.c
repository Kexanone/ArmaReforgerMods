//------------------------------------------------------------------------------------------------
//! Data class for reconnecting players
modded class SCR_ReconnectData
{
	Faction m_pFPM_PlayerFaction;
}

//------------------------------------------------------------------------------------------------
modded class SCR_ReconnectComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	override protected notnull SCR_ReconnectData StoreData(int playerId)
	{
		SCR_ReconnectData data = super.StoreData(playerId);
		
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		if (!playerController)
			return data;
		
		SCR_PlayerFactionAffiliationComponent factionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (factionAffiliation)
			data.m_pFPM_PlayerFaction = factionAffiliation.FPM_GetReconnectedFaction();
		
		return data;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void ApplyData(int playerId, notnull SCR_ReconnectData data)
	{
		super.ApplyData(playerId, data);
		
		if (!data.m_pFPM_PlayerFaction)
			return;
		
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		if (!playerController)
			return;
		
		SCR_PlayerFactionAffiliationComponent factionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (!factionAffiliation)
			return;
		
		factionAffiliation.RequestFaction(data.m_pFPM_PlayerFaction);
	}
}
