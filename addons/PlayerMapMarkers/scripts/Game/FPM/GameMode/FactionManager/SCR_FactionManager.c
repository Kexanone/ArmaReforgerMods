//------------------------------------------------------------------------------------------------
modded class SCR_FactionManager : FactionManager
{
	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		
		SCR_PlayerFactionAffiliationComponent playerFactionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (playerFactionAffiliation)
			playerFactionAffiliation.FPM_UpdateReconnectedFaction();
		
		super.OnPlayerDisconnected(playerId, cause, timeout);
	}
}
