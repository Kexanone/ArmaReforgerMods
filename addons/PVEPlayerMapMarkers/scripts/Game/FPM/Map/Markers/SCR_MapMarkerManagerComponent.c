//------------------------------------------------------------------------------------------------
modded class SCR_MapMarkerManagerComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	//! Copy of vanilla SetStreamRulesForPlayer, but added exception handling when marker is null
	override void SetStreamRulesForPlayer(int playerID)
	{			
		foreach (SCR_MapMarkerEntity dynamicMarker : m_aDynamicMarkers)
		{
			// FPM: Added exception handling
			if (!dynamicMarker)
				continue;
			
			Faction markerFaction = dynamicMarker.GetFaction();
			
			if (!markerFaction || markerFaction == SCR_FactionManager.SGetPlayerFaction(playerID))
				HandleStreamOut(dynamicMarker, GetGame().GetPlayerManager().GetPlayerController(playerID), false);
			else
				HandleStreamOut(dynamicMarker, GetGame().GetPlayerManager().GetPlayerController(playerID), true);
		};
	}
}
