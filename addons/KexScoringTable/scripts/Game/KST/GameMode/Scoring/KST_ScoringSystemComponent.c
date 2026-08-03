//------------------------------------------------------------------------------------------------
class KST_ScoringSystemComponentClass : SCR_ScoringSystemComponentClass
{
}

//------------------------------------------------------------------------------------------------
class KST_ScoringSystemComponent : SCR_ScoringSystemComponent
{
	[Attribute("2", UIWidgets.EditBox, "Soft kill score multiplier", category: "Scoring: Multipliers")]
	protected int m_iSoftKillScoreMultiplier;
	
	[Attribute("3", UIWidgets.EditBox, "Armor kill score multiplier", category: "Scoring: Multipliers")]
	protected int m_iArmorKillScoreMultiplier;
	
	[Attribute("5", UIWidgets.EditBox, "Air kill score multiplier", category: "Scoring: Multipliers")]
	protected int m_iAirKillScoreMultiplier;
	
	//------------------------------------------------------------------------------------------------
	override protected int CalculateScore(SCR_ScoreInfo info)
	{
		int score = super.CalculateScore(info);
		score += info.m_iKST_SoftKills * m_iSoftKillScoreMultiplier;
		score += info.m_iKST_ArmorKills * m_iArmorKillScoreMultiplier;
		score += info.m_iKST_AirKills * m_iAirKillScoreMultiplier;
		
		if (score < 0)
			return 0;
		
		return score;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnControllableDestroyed(notnull SCR_InstigatorContextData instigatorContextData)
	{
		super.OnControllableDestroyed(instigatorContextData);

		IEntity victim = instigatorContextData.GetVictimEntity();
		if (!victim)
			return;

		if (Vehicle.Cast(victim))
			OnVehicleDestroyed(instigatorContextData);

		if (ChimeraCharacter.Cast(victim))
			OnCharacterKilled(instigatorContextData);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnVehicleDestroyed(notnull SCR_InstigatorContextData instigatorContextData)
	{
		Instigator instigator = instigatorContextData.GetInstigator();	
	
		if (instigator.GetInstigatorType() != InstigatorType.INSTIGATOR_PLAYER)
			return;
		
		if (instigatorContextData.GetKillerCharacterControlType() == SCR_ECharacterControlType.POSSESSED_AI)
			return;

		Vehicle vehicle = Vehicle.Cast(instigatorContextData.GetVictimEntity());
		if (!vehicle)
			return;

		int playerID = instigatorContextData.GetInstigator().GetInstigatorPlayerID();
		if (playerID == 0)
			return;

		Faction playerFaction = SCR_FactionManager.SGetPlayerFaction(playerID);
		if (!playerFaction)
			return;

		Faction vehicleFaction = vehicle.GetFaction();
		// If vehicle does not have any affiliated faction, use it's default faction
		if (!vehicleFaction)
			vehicleFaction = vehicle.GetDefaultFaction();
		
		int count = 1;
		if (!vehicleFaction || playerFaction.IsFactionFriendly(vehicleFaction))
			count = -1;
		
		SCR_EditableEntityComponent editableVehicle = SCR_EditableEntityComponent.GetEditableEntity(vehicle);
		if (!editableVehicle)
			return;
				
		SCR_EditableEntityUIInfo info = SCR_EditableEntityUIInfo.Cast(editableVehicle.GetInfo());
		if (!info)
			return;
		
		KST_AddVehicleKill(instigator.GetInstigatorPlayerID(), info, count);	
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCharacterKilled(notnull SCR_InstigatorContextData instigatorContextData)
	{
		super.OnPlayerKilled(instigatorContextData);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Disable player killed handling, as we handle all characters via OnCharacterKilled
	protected override void OnPlayerKilled(notnull SCR_InstigatorContextData instigatorContextData)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	//! Ignore invalid player IDs comming from AI
	override void AddDeath(int playerId, int count = 1)
	{
		if (!m_pGameMode.IsMaster())
			return;
		
		if (playerId <= 0)
			return;
		
		super.AddDeath(playerId, count);
	}

	//------------------------------------------------------------------------------------------------
	//! Add count x vehicle kills to playerId's score info and their affiliated faction's.
	//! Server-only, propagated to all clients via BC reliable RPC.
	//! \param[in] playerId
	//! \param[in] count
	void KST_AddVehicleKill(int playerId, SCR_EditableEntityUIInfo info, int count = 1)
	{
		// Server only
		if (!m_pGameMode.IsMaster())
			return;
		
		KST_EScoreType type = KST_GetVehilceScoreType(info);
		int factionIdx = GetPlayerFactionIndex(playerId);
		RpcDo_KST_AddKill(playerId, factionIdx, type, count);
		Rpc(RpcDo_KST_AddKill, playerId, factionIdx, type, count);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	private void RpcDo_KST_AddKill(int playerId, int factionIdx, KST_EScoreType type, int count)
	{
		SCR_ScoreInfo playerScore = m_mPlayerScores[playerId];
		
		SCR_ScoreInfo factionScore;
		Faction faction = GetFactionByIndex(factionIdx);
		if (faction)
			factionScore = m_mFactionScores[faction];
		
		switch (type)
		{
			case KST_EScoreType.INFANTRY:
			{
				playerScore.m_iKills += count;
				
				if (factionScore)
					factionScore.m_iKills += count;
				
				break;
			}
			case KST_EScoreType.SOFT:
			{
				playerScore.m_iKST_SoftKills += count;
				
				if (factionScore)
					factionScore.m_iKST_SoftKills += count;
				
				break;
			}
			case KST_EScoreType.ARMOR:
			{
				playerScore.m_iKST_ArmorKills += count;
				
				if (factionScore)
					factionScore.m_iKST_ArmorKills += count;
				
				break;
			}
			case KST_EScoreType.AIR:
			{
				playerScore.m_iKST_AirKills += count;
				
				if (factionScore)
					factionScore.m_iKST_AirKills += count;
				
				break;
			}
		}
		
		OnPlayerScoreChanged(playerId, playerScore);
		
		if (factionScore)
			OnFactionScoreChanged(faction, factionScore);
	}
	
	//------------------------------------------------------------------------------------------------
	protected KST_EScoreType KST_GetVehilceScoreType(SCR_EditableEntityUIInfo info)
	{
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_CAR))
			return KST_EScoreType.SOFT;
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_TRUCK))
			return KST_EScoreType.SOFT;
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_TURRET))
			return KST_EScoreType.SOFT;
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_APC))
			return KST_EScoreType.ARMOR;
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_AIRPLANE))
			return KST_EScoreType.AIR;
		if (info.HasEntityLabel(EEditableEntityLabel.VEHICLE_HELICOPTER))
			return KST_EScoreType.AIR;
		
		return KST_EScoreType.NONE;
	}
}
