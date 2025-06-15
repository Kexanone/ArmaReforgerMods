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
	
		marker.SetPlayerId(playerId);
			
		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
		charCtrl.m_OnLifeStateChanged.Insert(marker.OnLifeStateChanged);
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(player.FindComponent(SCR_CompartmentAccessComponent));
		if (!compartmentAccess)
			return;

		compartmentAccess.GetOnPlayerCompartmentEnter().Insert(OnPlayerEnterCompartment);
		compartmentAccess.GetOnPlayerCompartmentExit().Insert(OnPlayerExitCompartment);
		
		
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		if (!playerController)
			return;
		
		SCR_PlayerFactionAffiliationComponent factionAffiliation = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (!factionAffiliation)
			return;
		
		Faction faction = factionAffiliation.GetAffiliatedFaction();
		if (faction)
			marker.SetFaction(faction);
		
		SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.GetInstance();
		SCR_AIGroup aiGroup = groupManager.GetPlayerGroup(playerId);
		if (aiGroup)
			marker.SetGroupId(aiGroup.GetGroupID());
		
		marker.SetGlobalText(GetPlayerNameWithRank(playerId, player));
		marker.SetGlobalSymbolIcons(EMilitarySymbolIcon.INFANTRY);
		marker.SetGlobalVisible(true);
		m_mPlayerMarkers.Insert(player, marker);
	}
	
	//------------------------------------------------------------------------------------------------
	string GetPlayerNameWithRank(int playerId, IEntity player)
	{
		string name = GetGame().GetPlayerManager().GetPlayerName(playerId);
		
		string rank = SCR_CharacterRankComponent.GetCharacterRankNameShort(player);
		if (rank)
			return string.Format("%1 %2", rank, name);
		
		return name;
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
				
		array<BaseCompartmentSlot> compartments = {};
		compartmentManager.GetCompartments(compartments);
		array<IEntity> occupants = {};
		
		foreach (BaseCompartmentSlot compartment : compartments)
		{
			IEntity occupant = compartment.GetOccupant();
			// Switching seats can lead to an occupant being in two compartments simultaneously
			// So we have to make sure to only insert them once
			if (occupant && occupant != excludedOccupant && !occupants.Contains(occupant))
				occupants.Insert(occupant);
		}

		int passengerCount = occupants.Count() - 1;
		
		IEntity effectiveCommander;
		foreach (IEntity occupant : occupants)
		{
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
				UpdateEffectiveCommanderMarker(marker, GetPlayerNameWithRank(playerId, occupant), vehicle, passengerCount);
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
		
		EMilitarySymbolDimension dimension = EMilitarySymbolDimension.LAND;
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
			dimension = EMilitarySymbolDimension.AIR;
			icons = EMilitarySymbolIcon.ROTARY_WING;
		}
		else if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_AIRPLANE))
		{
			dimension = EMilitarySymbolDimension.AIR;
			icons = EMilitarySymbolIcon.FIXED_WING;
		}
		
		marker.SetGlobalSymbolIcons(icons, dimension);
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
		
		marker.SetGlobalText(GetPlayerNameWithRank(playerId, playerCharacter));
		marker.SetGlobalSymbolIcons(EMilitarySymbolIcon.INFANTRY);
		marker.SetGlobalVisible(true);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnRankChanged(SCR_ECharacterRank oldRank, SCR_ECharacterRank newRank, notnull IEntity owner, bool silent)
	{
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(owner.FindComponent(SCR_CompartmentAccessComponent));
		if (!compartmentAccess)
			return;
		
		Vehicle vehicle = Vehicle.Cast(compartmentAccess.GetVehicle());
		if (vehicle)
		{
			UpdatePlayerMarkersInVehicle(vehicle);
			return;
		}

		FPM_MapMarkerPlayer marker = m_mPlayerMarkers[owner];
		if (!marker)
			return;
		
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(owner);
		marker.SetGlobalText(GetPlayerNameWithRank(playerId, owner));
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerFactionChanged(int playerId, SCR_PlayerFactionAffiliationComponent playerComponent, Faction faction)
	{
		IEntity playerChar = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if (!playerChar)
			return;
		
		FPM_MapMarkerPlayer marker = m_mPlayerMarkers[playerChar];
		if (!marker)
			return;
		
		marker.SetFaction(faction);
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
	protected void OnGroupJoined(SCR_AIGroup group, int playerID)
	{
		IEntity ent = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		if (ent)
		{
			FPM_MapMarkerPlayer marker = m_mPlayerMarkers.Get(ent);
			if (marker)
				marker.SetGroupId(group.GetGroupID());
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected  void OnGroupLeft(SCR_AIGroup group, int playerID)
	{
		IEntity ent = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		if (ent)
		{
			FPM_MapMarkerPlayer marker = m_mPlayerMarkers.Get(ent);
			if (marker)
				marker.SetGroupId(-1);
		}
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
		SCR_CharacterRankComponent.s_OnRankChanged.Insert(OnRankChanged);
		
		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (factionManager)
			factionManager.GetOnPlayerFactionChanged_S().Insert(OnPlayerFactionChanged);
		
		SCR_AIGroup.GetOnPlayerAdded().Insert(OnGroupJoined);
		SCR_AIGroup.GetOnPlayerRemoved().Insert(OnGroupLeft);
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
