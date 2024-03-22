//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_MapMarkerTitle()]
class FPM_MapMarkerEntryPlayer : SCR_MapMarkerEntryDynamic
{
	protected ref map<IEntity, FPM_MapMarkerPlayer> m_mPlayerMarkers = new map<IEntity, FPM_MapMarkerPlayer>();
	
	//------------------------------------------------------------------------------------------------
	//! Register marker here so it can be fetched from the map
	void CreateMarker(int playerId, IEntity player)
	{
		if (!player || m_mPlayerMarkers.Contains(player))
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
		
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(player);
		if (!playerCharacter)
			return;
		
		Faction faction = playerCharacter.GetFaction();
		if (faction)
			marker.SetFaction(faction);
		
		
		marker.SetGlobalText(GetGame().GetPlayerManager().GetPlayerName(playerId));
		marker.SetGlobalVisible(true);
		m_mPlayerMarkers.Insert(player, marker);
	}
	
	//------------------------------------------------------------------------------------------------
	void DeleteMarker(IEntity player)
	{
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers.Get(player);
		if (marker)
			m_MarkerMgr.RemoveDynamicMarker(marker);
		
		m_mPlayerMarkers.Remove(player);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerConnected(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;
		
		playerController.m_FPM_OnControlledEntityChangedServer.Insert(OnControlledEntityChangedServer);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnControlledEntityChangedServer(int playerId, IEntity from, IEntity to)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController || playerController.IsPossessing())
			return;
		
		// Delete marker of previous body first
		DeleteMarker(from);
		CreateMarker(playerId, to);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdatePlayerMarkersInVehicle(Vehicle vehicle, IEntity excludedOccupant = null)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;
				
		array<IEntity> occupants = {};
		compartmentManager.GetOccupants(occupants);
		
		int passengerCount = occupants.Count() - 1;
		if (excludedOccupant)
			passengerCount--;
		
		IEntity effectiveCommander;
		foreach (IEntity occupant : occupants)
		{
			if (occupant == excludedOccupant)
				continue;
			
			int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(occupant);
			if (!playerId)
				continue;
			
			if (!effectiveCommander)
				effectiveCommander = occupant;
			
			FPM_MapMarkerPlayer marker = m_mPlayerMarkers[occupant];
			if (!marker)
				continue;
			
			bool isEffectiveCommander = (occupant == effectiveCommander);
			marker.SetGlobalVisible(isEffectiveCommander);
			
			if (isEffectiveCommander)
				UpdateEffectiveCommanderMarker(marker, GetGame().GetPlayerManager().GetPlayerName(playerId), vehicle, passengerCount);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateEffectiveCommanderMarker(FPM_MapMarkerPlayer marker, string commanderName, Vehicle vehicle, int passengerCount)
	{
		SCR_EditableVehicleComponent edit = SCR_EditableVehicleComponent.Cast(vehicle.FindComponent(SCR_EditableVehicleComponent));
		if (!edit)
			return;;
				
		if (passengerCount > 0)
		{
			marker.SetGlobalText(string.Format("%1 (%2) +%3", edit.GetDisplayName(), commanderName, passengerCount));
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
		
		UpdatePlayerMarkersInVehicle(vehicle, playerCharacter);
		
		// Reset the marker for the player that exits
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(playerCharacter);
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers[playerCharacter];
		if (!marker)
			return;
		
		marker.SetGlobalText(GetGame().GetPlayerManager().GetPlayerName(playerId));
		marker.SetGlobalSymbolIcons(EMilitarySymbolIcon.INFANTRY);
		marker.SetGlobalVisible(true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		DeleteMarker(player);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerDeleted(int playerId, IEntity player)
	{		
		DeleteMarker(player);
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
		
		gameMode.GetOnPlayerConnected().Insert(OnPlayerConnected);
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
