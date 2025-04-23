//------------------------------------------------------------------------------------------------
modded class SCR_ConsumableItemComponent : SCR_GadgetComponent
{
	protected ChimeraCharacter m_pBP_TargetCharacter;
	
	//------------------------------------------------------------------------------------------------
	//! Broadcast the removal of the lock
	override protected void SetHealedGroup(EBandagingAnimationBodyParts group, bool healed)
	{
		if (!Replication.IsServer())
			return;
		
		if (!m_pBP_TargetCharacter)
			m_pBP_TargetCharacter = ChimeraCharacter.Cast(GetTargetCharacter());
		
		RplId targetCharID = Replication.FindItemId(m_pBP_TargetCharacter);
		RpcDo_BP_SetHealedGroupBroadcast(targetCharID, group, healed);
		Rpc(RpcDo_BP_SetHealedGroupBroadcast, targetCharID, group, healed);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_BP_SetHealedGroupBroadcast(RplId targetCharID, EBandagingAnimationBodyParts group, bool healed)
	{
		m_iStoredHealedGroup = group * healed;
		
		if (!m_pBP_TargetCharacter)
			m_pBP_TargetCharacter = ChimeraCharacter.Cast(Replication.FindItem(targetCharID));
		
		if (!m_pBP_TargetCharacter)
			return;
		
		SCR_CharacterDamageManagerComponent targetDamageManager = SCR_CharacterDamageManagerComponent.Cast(m_pBP_TargetCharacter.GetDamageManager());
		if (!targetDamageManager)
			return;
		
		targetDamageManager.SetGroupIsBeingHealed(targetDamageManager.FindAssociatedHitZoneGroup(group), healed);
	}
}
