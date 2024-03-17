//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_MapMarkerTitle()]
class FPM_MapMarkerEntryPlayer : SCR_MapMarkerEntryDynamic
{
	protected ref map<int, FPM_MapMarkerPlayer> m_mPlayerMarkers = new map<int, FPM_MapMarkerPlayer>();
	
	//------------------------------------------------------------------------------------------------
	//! Register marker here so it can be fetched from the map
	void CreateMarker(int playerId)
	{
		if (m_mPlayerMarkers.Contains(playerId))
			return;
		
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if (!player)
			return;
		
		FPM_MapMarkerPlayer marker = FPM_MapMarkerPlayer.Cast(m_MarkerMgr.InsertDynamicMarker(SCR_EMapMarkerType.FPM_PLAYER, player));
		if (!marker)
			return;
		
		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
		charCtrl.m_OnLifeStateChanged.Insert(marker.OnLifeStateChanged);
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(player.FindComponent(SCR_CompartmentAccessComponent));
		if (!compartmentAccess)
			return;

		compartmentAccess.GetOnPlayerCompartmentEnter().Insert(OnPlayerEnterCompartment);
		compartmentAccess.GetOnPlayerCompartmentExit().Insert(OnPlayerExitCompartment);
		
		SCR_GroupIdentityCore core = SCR_GroupIdentityCore.Cast(SCR_GroupIdentityCore.GetInstance(SCR_GroupIdentityCore));
		if (!core)
			return;
		
		Faction faction = SCR_FactionManager.SGetPlayerFaction(playerId);
		if (faction)
			marker.SetFaction(faction);
		
		
		marker.SetGlobalText(GetGame().GetPlayerManager().GetPlayerName(playerId));
		marker.SetGlobalVisible(true);
		m_mPlayerMarkers.Insert(playerId, marker);
	}
	
	//------------------------------------------------------------------------------------------------
	void DeleteMarker(int playerId)
	{
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers.Get(playerId);
		if (marker)
			m_MarkerMgr.RemoveDynamicMarker(marker);
		
		m_mPlayerMarkers.Remove(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerSpawned(int playerId, IEntity player)
	{
		// Delete marker of previous body first
		DeleteMarker(playerId);
		CreateMarker(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdatePlayerMarkersInVehicle(Vehicle vehicle)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;
				
		array<IEntity> occupants = {};
		compartmentManager.GetOccupants(occupants);
		
		IEntity effectiveCommander;
		foreach (IEntity occupant : occupants)
		{
			int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(occupant);
			if (!playerId)
				continue;
			
			if (!effectiveCommander)
				effectiveCommander = occupant;
			
			FPM_MapMarkerPlayer marker = m_mPlayerMarkers[playerId];
			bool isEffectiveCommander = (occupant == effectiveCommander);
			marker.SetGlobalVisible(isEffectiveCommander);
			
			if (isEffectiveCommander)
				UpdateEffectiveCommanderMarker(marker, GetGame().GetPlayerManager().GetPlayerName(playerId), vehicle, occupants.Count() - 1);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateEffectiveCommanderMarker(FPM_MapMarkerPlayer marker, string commanderName, Vehicle vehicle, int extraOccupantCount)
	{
		SCR_EditableVehicleComponent edit = SCR_EditableVehicleComponent.Cast(vehicle.FindComponent(SCR_EditableVehicleComponent));
		if (!edit)
			return;;
				
		if (extraOccupantCount > 0)
		{
			marker.SetGlobalText(string.Format("%1 (%2) +%3", edit.GetDisplayName(), commanderName, extraOccupantCount));
		}
		else
		{
			marker.SetGlobalText(string.Format("%1 (%2)", edit.GetDisplayName(), commanderName));
		};
		
		EMilitarySymbolIcon icons = 0;
				
		SCR_EditableEntityUIInfo entityUIInfo = SCR_EditableEntityUIInfo.Cast(edit.GetInfo());
		if (!entityUIInfo)
			return;
		
		if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_CAR) || entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_TRUCK))
		{
			icons = EMilitarySymbolIcon.INFANTRY | EMilitarySymbolIcon.MOTORIZED;
		}	
		else if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_APC))
		{
			icons = EMilitarySymbolIcon.INFANTRY | EMilitarySymbolIcon.ARMOR;		
		}
		else if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_HELICOPTER))
		{
			icons = EMilitarySymbolIcon.ROTARY_WING;
		}
		else if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_AIRPLANE))
		{
			icons = EMilitarySymbolIcon.FIXED_WING;
		};
		
		marker.SetGlobalSymbolIcons(icons);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerEnterCompartment(ChimeraCharacter playerCharacter, IEntity compartmentEntity)
	{
		Vehicle vehicle = Vehicle.Cast(compartmentEntity.GetRootParent());
		if (vehicle)
			UpdatePlayerMarkersInVehicle(vehicle);
	}

	//------------------------------------------------------------------------------------------------
	void OnPlayerExitCompartment(ChimeraCharacter playerCharacter, IEntity compartmentEntity)
	{
		Vehicle vehicle = Vehicle.Cast(compartmentEntity.GetRootParent());
		if (!vehicle)
			return;
		
		UpdatePlayerMarkersInVehicle(vehicle);
		
		// Reset the marker for the player that exits
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(playerCharacter);
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers[playerId];
		marker.SetGlobalText(GetGame().GetPlayerManager().GetPlayerName(playerId));
		marker.SetGlobalSymbolIcons(EMilitarySymbolIcon.INFANTRY);
		marker.SetGlobalVisible(true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{		
		DeleteMarker(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerDeleted(int playerId, IEntity player)
	{		
		DeleteMarker(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Update marker target, will trigger creation of a marker if within map
	protected void UpdateMarkerTarget(int playerId)
	{		
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers.Get(playerId);
	} 
	
	//------------------------------------------------------------------------------------------------
	override SCR_EMapMarkerType GetMarkerType()
	{
	 	return SCR_EMapMarkerType.FPM_PLAYER;
	}
	
	//------------------------------------------------------------------------------------------------
	override void InitServerLogic()
	{
		super.InitServerLogic();
		
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;
		
		gameMode.GetOnPlayerSpawned().Insert(OnPlayerSpawned);
		gameMode.GetOnPlayerDisconnected().Insert(OnPlayerDisconnected);
		gameMode.GetOnPlayerDeleted().Insert(OnPlayerDeleted);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnMapLayerChangedDynamic(notnull SCR_MapMarkerDynamicWComponent widgetComp, int layerID)
	{
		super.OnMapLayerChangedDynamic(widgetComp, layerID);
		
		FPM_MapMarkerPlayerComponent fpmWidgetComp = FPM_MapMarkerPlayerComponent.Cast(widgetComp);
		if (fpmWidgetComp)
			fpmWidgetComp.UpdateTextVisibility();
	}
}