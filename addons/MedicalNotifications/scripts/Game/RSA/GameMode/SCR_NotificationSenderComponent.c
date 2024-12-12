//-----------------------------------------------------------------------------------------------------------
modded class SCR_NotificationSenderComponent : SCR_BaseGameModeComponent
{
	[Attribute(desc: "")]
	protected ref array<ref RSA_InstigatorEventNotificationKeys> m_aInstigatorEventNotificationKeys;
	
	protected ref map<RSA_EInstigatorEvent,ref RSA_InstigatorEventNotificationKeys> m_mInstigatorEventNotificationKeysMap;
	
	void OnControllableIncapacitated(IEntity entity, IEntity instigator)
	{
		OnInstigatorEvent(RSA_EInstigatorEvent.INCAPACITATED, entity, instigator);
	};
	
	void OnControllableRevived(IEntity entity, IEntity instigator)
	{
		OnInstigatorEvent(RSA_EInstigatorEvent.REVIVED, entity, instigator);
	};
	
	void OnInstigatorEvent(RSA_EInstigatorEvent eventKey, IEntity entity, IEntity instigator)
	{
		if (!entity)
			return;
		
		RplComponent entityRpl = RplComponent.Cast(entity.FindComponent(RplComponent));
		RplComponent instigatorRpl;
		
		if (instigator)
			instigatorRpl = RplComponent.Cast(instigator.FindComponent(RplComponent));
		
		RplId entityId = RplId.Invalid();
		RplId instigatorId = RplId.Invalid();
		
		if (entityRpl)
			entityId = entityRpl.Id();
		
		if (instigatorRpl)
			instigatorId = instigatorRpl.Id();
		
		OnInstigatorEventBroadCast(eventKey, entityId, instigatorId);
		Rpc(OnInstigatorEventBroadCast, eventKey, entityId, instigatorId);
	};
	
	void InitializeInstigatorEventNotificationKeys()
	{
		m_mInstigatorEventNotificationKeysMap = new map<RSA_EInstigatorEvent,ref RSA_InstigatorEventNotificationKeys>;
		
		foreach (RSA_InstigatorEventNotificationKeys notificationKeys : m_aInstigatorEventNotificationKeys)
		{
			m_mInstigatorEventNotificationKeysMap.Set(notificationKeys.m_InstigatorEventKey, notificationKeys);
		};
	};
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void OnInstigatorEventBroadCast(RSA_EInstigatorEvent eventKey, RplId entityId, RplId instigatorId)
	{
		IEntity entity;
		IEntity instigator;
		
		RplComponent entityRpl = RplComponent.Cast(Replication.FindItem(entityId));
		RplComponent instigatorRpl = RplComponent.Cast(Replication.FindItem(instigatorId));
		
		if (entityRpl)
			entity = entityRpl.GetEntity();
		
		if (instigatorRpl)
			instigator = instigatorRpl.GetEntity();
			
		//~	No entity destroyed
		if (!entity)
			return;
		
		if (!m_mInstigatorEventNotificationKeysMap)
			InitializeInstigatorEventNotificationKeys();
		
		RSA_InstigatorEventNotificationKeys notificationKeys = m_mInstigatorEventNotificationKeysMap[eventKey];
		
		bool isUnlimitedEditorOpened = false;
		
		//~ Check if player has unlimited editor and if the editor is open
		SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
		if (editorManager)
			isUnlimitedEditorOpened = !editorManager.IsLimited() && editorManager.IsOpened();
		
		//~ Killfeed is disabled and unlimited editor is not open
		if (!isUnlimitedEditorOpened && m_iKillFeedType == EKillFeedType.DISABLED)
			return;
		
		int playerId = SCR_PossessingManagerComponent.GetPlayerIdFromControlledEntity(entity);
		if (playerId <= 0)
			return;
		
		//~ Check if killed player message can be seen if Limited editor (or editor not open)
		if (!isUnlimitedEditorOpened && m_iReceiveKillFeedType != EKillFeedReceiveType.ALL)
		{
			Faction localPlayerFaction = m_FactionManager.GetLocalPlayerFaction();

			//~ No local faction so don't show killfeed
			if (!localPlayerFaction)
				return;
			
			int localPlayerID = SCR_PlayerController.GetLocalPlayerId();
			Faction killedPlayerFaction = m_FactionManager.GetPlayerFaction(playerId);
			
			switch (m_iReceiveKillFeedType)
			{
				//~ check if in group
				case EKillFeedReceiveType.GROUP_ONLY :
				{
					//~ Check if local player is not the same as killed otherwise they are always in the same group
					if (localPlayerID != playerId)
					{
						//~ Factions not friendly so don't show killfeed
						if (!localPlayerFaction.IsFactionFriendly(killedPlayerFaction))
							return;
						
						//~ No group manager so don't send
						SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.GetInstance();
						if (!groupManager)
							return;
						
						SCR_AIGroup localPlayerGroup = groupManager.GetPlayerGroup(localPlayerID);
						
						//~ If not in group or not in the same group do not send
						if (!localPlayerGroup || !localPlayerGroup.IsPlayerInGroup(playerId))
							return;
					}
					
					break;
				}
				//~ Check if the same faction
				case EKillFeedReceiveType.SAME_FACTION_ONLY :
				{
					//~ Check if local player is not the same as killed otherwise they are always the same faction
					if (localPlayerID != playerId)
					{
						//~ If no local faction or if not the same faction do not show killfeed
						if (!localPlayerFaction || localPlayerFaction != killedPlayerFaction)
							return;
						
						//~ If Faction is hostile towards itself still do not show killfeed (DM)
						if (!localPlayerFaction.IsFactionFriendly(localPlayerFaction))
							return;
					}
					
					break;
				}
				//~ Check if allies
				case EKillFeedReceiveType.ALLIES_ONLY :
				{
					//~ Check if local player is not the same as killed otherwise they are always allied
					if (localPlayerID != playerId)
					{
						//~ Factions not friendly so don't show killfeed
						if (!localPlayerFaction || !localPlayerFaction.IsFactionFriendly(killedPlayerFaction))
							return;
					}
					
					break;
				}
				//~ Check if enemies
				case EKillFeedReceiveType.ENEMIES_ONLY :
				{
					//~ If local player killed it is never an enemy
					if (localPlayerID == playerId)
						return;
					
					//~ Factions friendly so don't show killfeed
					if (localPlayerFaction.IsFactionFriendly(killedPlayerFaction))
						return;
					
					break;
				}
			}
		}
		
		//~ Never show killer so simply show player died if limited editor
		if (!isUnlimitedEditorOpened && m_iKillFeedType == EKillFeedType.UNKNOWN_KILLER)
		{
			SCR_NotificationsComponent.SendLocal(notificationKeys.m_UnknownOnPlayerNotificationKey, playerId);
			return;
		}
		
		int killerId;
		if (entity == instigator)
			killerId = playerId;
		else 
			killerId = SCR_PossessingManagerComponent.GetPlayerIdFromControlledEntity(instigator);

		SCR_EditableCharacterComponent killerEditableCharacterComponent;
		//~ If there is a killer and the killer is not the player itself
		if (instigator && killerId <= 0)
		{
			killerEditableCharacterComponent = SCR_EditableCharacterComponent.Cast(instigator.FindComponent(SCR_EditableCharacterComponent));
		
			//~ Killer was not character so get killer in vehicle
			if (!killerEditableCharacterComponent)
				killerEditableCharacterComponent = GetKillerFromVehicle(instigator, instigator.IsInherited(Vehicle));
			
			if (killerEditableCharacterComponent)
				killerId = SCR_PossessingManagerComponent.GetPlayerIdFromControlledEntity(killerEditableCharacterComponent.GetOwner());
		}

		bool playerIsPossessed = false; 
		bool killerIsPossessed = false; 
		
		//~ Check if player or killer where possessed
		SCR_PossessingManagerComponent possesionManager = SCR_PossessingManagerComponent.GetInstance();
		if (possesionManager)
		{
			playerIsPossessed = possesionManager.IsPossessing(playerId);
			
			if (killerId > 0)
				killerIsPossessed = possesionManager.IsPossessing(killerId);
		}
			
		//Death notification	
		//Suicide	
		if (playerId == killerId || !instigator)
		{			
			//Player Suicide
			if (!playerIsPossessed)
				SCR_NotificationsComponent.SendLocal(notificationKeys.m_UnknownOnPlayerNotificationKey, playerId);
			//Possessed Suicide
			else 
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_UnknownOnPossessedNotificationKey, playerId);
		}
		//If killed by other player
		else if (killerId > 0)
		{
			//Player killed player
			if (!playerIsPossessed && !killerIsPossessed)
			{
				SCR_NotificationsComponent.SendLocal(notificationKeys.m_PlayerOnPlayerNotificationKey, killerId, playerId);
			}
			//Possesed player killed by other player (Show player killed by NPC and for GM: possesed GM killed player)
			else if (!playerIsPossessed && killerIsPossessed)
			{		
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_PossessedOnPlayerNotificationKey, killerId, playerId);
				SCR_NotificationsComponent.SendLocalLimitedEditor(notificationKeys.m_AIOnPlayerNotificationKey, killerId, playerId);
			}
			//Player killed possessed player (Show to GM only)
			else if (playerIsPossessed && !killerIsPossessed)
			{
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_PlayerOnPossessedNotificationKey, killerId, playerId);
			}
			//Possessed AI Killed Possessed AI (GM Only)
			else
			{
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_PossessedOnPossessedNotificationKey, killerId, playerId);
			}
		}
		//Killed by NPC
		else if (killerEditableCharacterComponent)
		{
			int killerRplId = Replication.FindId(killerEditableCharacterComponent);
			
			if (!playerIsPossessed)
				SCR_NotificationsComponent.SendLocal(notificationKeys.m_AIOnPlayerNotificationKey, killerRplId, playerId);
			else 
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_AIOnPossessedNotificationKey, killerRplId, playerId);
		}
		//Unknown killer
		else 
		{
			if (!playerIsPossessed)
				SCR_NotificationsComponent.SendLocal(notificationKeys.m_UnknownOnPlayerNotificationKey, playerId);
			else 
				SCR_NotificationsComponent.SendLocalUnlimitedEditor(notificationKeys.m_UnknownOnPossessedNotificationKey, playerId);
		}
	}
	
	// Old BI method that no longer exists in the vanilla code base
	//---- REFACTOR NOTE START: Checks if character was killed by a vehicle. Don't think this is needed any more ----
	//------------------------------------------------------------------------------------------------
	protected SCR_EditableCharacterComponent GetKillerFromVehicle(IEntity veh, bool pilot)
	{
		BaseCompartmentManagerComponent compartmentManager = BaseCompartmentManagerComponent.Cast(veh.FindComponent(BaseCompartmentManagerComponent));
			
		if (!compartmentManager)
			return null;
		
		array<BaseCompartmentSlot> compartments = {};
		
		for (int i = 0, compart = compartmentManager.GetCompartments(compartments); i < compart; i++)
		{
			BaseCompartmentSlot slot = compartments[i];
			
			if (pilot && slot.Type() == PilotCompartmentSlot)
			{
				if (slot.GetOccupant())
					return SCR_EditableCharacterComponent.Cast(slot.GetOccupant().FindComponent(SCR_EditableCharacterComponent));
			}
			else if (!pilot && slot.Type() == TurretCompartmentSlot)
			{
				if (slot.GetOccupant())
					return SCR_EditableCharacterComponent.Cast(slot.GetOccupant().FindComponent(SCR_EditableCharacterComponent));
			}
		}
		
		return null;
	}
	
	//---- REFACTOR NOTE END ----
}
