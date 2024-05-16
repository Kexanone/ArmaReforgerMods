class KSC_FactionDominanceTriggerEntityClass : SCR_FactionDominanceTriggerEntityClass
{
}

class KSC_FactionDominanceTriggerEntity : SCR_FactionDominanceTriggerEntity
{
	void SetFriendlyRatioLimit(float ratio)
	{
		m_fFriendlyRatioLimit = ratio;
	}
}
