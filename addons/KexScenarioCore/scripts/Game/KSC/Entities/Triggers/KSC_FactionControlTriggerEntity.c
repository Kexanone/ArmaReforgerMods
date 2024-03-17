[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class KSC_FactionControlTriggerEntityClass : SCR_FactionControlTriggerEntityClass
{
};
class KSC_FactionControlTriggerEntity : SCR_FactionControlTriggerEntity
{
	void SetFriendlyRatioLimit(float ratio)
	{
		m_fFriendlyRatioLimit = ratio;
	}
}
