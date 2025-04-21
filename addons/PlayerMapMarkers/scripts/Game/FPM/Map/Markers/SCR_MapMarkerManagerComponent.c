//------------------------------------------------------------------------------------------------
modded class SCR_MapMarkerManagerComponent : SCR_BaseGameModeComponent
{
	[Attribute("0", desc: "Enable free-for-all (PvP) mode — markers only visible to groupmates", category: "FPM Markers")]
	protected bool m_bIsFreeForAll;

	void SetFreeForAll(bool state)
	{
		m_bIsFreeForAll = state;
	}

	bool IsFreeForAll()
	{
		return m_bIsFreeForAll;
	}

	//------------------------------------------------------------------------------------------------
	//! Similar to vanilla SetStreamRulesForPlayer, but added exception handling when marker is null and more advanced logic to determine whether to stream a marker
	override void SetStreamRulesForPlayer(int playerID)
	{
		foreach (SCR_MapMarkerEntity dynamicMarker : m_aDynamicMarkers)
		{
			// FPM: Added exception handling
			if (!dynamicMarker)
				continue;
			
			FPM_MapMarkerPlayer fpmMarker = FPM_MapMarkerPlayer.Cast(dynamicMarker);
			if (fpmMarker)
			{
				// If SCR_MapMarkerEntity is successfully cast to FPM_MapMarkerPlayer, handle it as such.
				PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerID);
				bool showMarker = FPM_ShouldShowMarkerForPlayer(fpmMarker, playerID);
				HandleStreamOut(fpmMarker, playerController, !showMarker);
			}
			else
			{
 				// Otherwise, handle the entity as a SCR_MapMarkerEntity as it is in vanilla.
				Faction markerFaction = dynamicMarker.GetFaction();
				
				if (!markerFaction || markerFaction == SCR_FactionManager.SGetPlayerFaction(playerID))
					HandleStreamOut(dynamicMarker, GetGame().GetPlayerManager().GetPlayerController(playerID), false);
				else
					HandleStreamOut(dynamicMarker, GetGame().GetPlayerManager().GetPlayerController(playerID), true);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Similar to vanilla SetMarkerStreamRules, but with more advanced logic to determine whether to stream a marker
	override void SetMarkerStreamRules(notnull SCR_MapMarkerEntity marker)
	{
		// If marker can be casted to FPM marker, we do our own handling
		FPM_MapMarkerPlayer fpmMarker = FPM_MapMarkerPlayer.Cast(marker);
		if (!fpmMarker)
			return super.SetMarkerStreamRules(marker);
		
		array<int> players = {};
		GetGame().GetPlayerManager().GetPlayers(players);
		
		foreach (int playerId : players)
		{
			PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
			bool showMarker = FPM_ShouldShowMarkerForPlayer(fpmMarker, playerId);
			HandleStreamOut(fpmMarker, playerController, !showMarker);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	// Decides if a marker should be shown to a given player based on factors such as faction, group, and ownership of the marker.
	static bool FPM_ShouldShowMarkerForPlayer(FPM_MapMarkerPlayer marker, int playerId)
	{
		int markerOwnerId = marker.GetPlayerId();
		
		Faction playerFaction = SCR_FactionManager.SGetPlayerFaction(playerId);
		if (!playerFaction)
			return false;
		
		Faction markerOwnerFaction = SCR_FactionManager.SGetPlayerFaction(markerOwnerId);
		if (!markerOwnerFaction)
			return false;
		
		// Always show the player their own position
		if (playerId == markerOwnerId)
			return true;

		SCR_MapMarkerManagerComponent markerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		if (!markerMgr)
			return false;

		if (markerMgr.IsFreeForAll())
		{
			SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.GetInstance();
			if (!groupManager)
				return false;

			SCR_AIGroup playerGroup = groupManager.GetPlayerGroup(playerId);
			SCR_AIGroup markerOwnerGroup = groupManager.GetPlayerGroup(markerOwnerId);

			return (playerGroup && markerOwnerGroup && playerGroup == markerOwnerGroup);
		}

		return playerFaction == markerOwnerFaction;
	}
}
