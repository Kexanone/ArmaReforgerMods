//------------------------------------------------------------------------------------------------
class KSC_ClearAreaTaskClass : KSC_AreaTriggerTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_ClearAreaTask : KSC_AreaTriggerTask
{
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, float radius = 50, float friendlyRatioThreshold = 0.528, array<LocalizedString> formatParams = null)
	{
		super.SetParams(targetFaction, radius, formatParams);
		KSC_FactionDominanceTriggerEntity trigger = KSC_FactionDominanceTriggerEntity.Cast(m_pTrigger);
		trigger.AddOwnerFaction(targetFaction.GetFactionKey());
		trigger.SetFriendlyRatioLimit(friendlyRatioThreshold);
	}
}
