//------------------------------------------------------------------------------------------------
//! Data class for reconnecting players
modded class SCR_ReconnectData
{
	Faction m_pFPM_PlayerFaction;
	
	//------------------------------------------------------------------------------------------------
	void SCR_ReconnectData(int playerId, IEntity entity)
	{
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		if (!playerController)
			return;
		
		SCR_PlayerFactionAffiliationComponent factionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (!factionAffiliation)
			return;
				
		m_pFPM_PlayerFaction = factionAffiliation.FPM_GetReconnectedFaction();
	}
}

//------------------------------------------------------------------------------------------------
modded class SCR_ReconnectComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (GetGame().InPlayMode() && Replication.IsServer())
			GetOnReconnect().Insert(FPM_OnPlayerReconnect);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FPM_OnPlayerReconnect(SCR_ReconnectData data)
	{
		if (!data.m_pFPM_PlayerFaction)
			return;
		
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(data.m_iPlayerId);
		if (!playerController)
			return;
		
		SCR_PlayerFactionAffiliationComponent factionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (!factionAffiliation)
			return;
		
		factionAffiliation.RequestFaction(data.m_pFPM_PlayerFaction);
	}
}
