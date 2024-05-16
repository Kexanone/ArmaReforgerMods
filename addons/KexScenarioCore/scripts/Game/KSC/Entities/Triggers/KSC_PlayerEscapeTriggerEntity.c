//------------------------------------------------------------------------------------------------
class KSC_PlayerEscapeTriggerEntityClass : SCR_BaseTriggerEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_PlayerEscapeTriggerEntity : SCR_BaseTriggerEntity
{
	[Attribute(defvalue: "0.66", uiwidget: UIWidgets.Slider, desc: "Minimum portion of alive players that are outside the area for the trigger to get activated.", params: "0 1 0.01")]
	protected float m_fEscapeRatioThreshold;
	
	protected int m_iPlayersInAreaCount = 0;
	protected bool m_bActivated = false;
	
	//------------------------------------------------------------------------------------------------
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (IsAlive(ent) && EntityUtils.IsPlayer(ent))
			m_iPlayersInAreaCount++;
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnQueryFinished(bool bIsEmpty)
	{
		// Only alive players count to the total count
		int playerCount = KSC_PlayerHelper.GetPlayers(true).Count();
		bool isActive = (playerCount > 0 && (1 - (float)m_iPlayersInAreaCount / playerCount >= m_fEscapeRatioThreshold));
		
		if (isActive && !m_bActivated)
		{
			OnActivate(null);
		}
		else if (!isActive && m_bActivated)
		{
			OnDeactivate(null);
		}
		
		m_iPlayersInAreaCount = 0;
		super.OnQueryFinished(bIsEmpty);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetEscapeRatioThreshold(float ratio)
	{
		m_fEscapeRatioThreshold = ratio;
	}
}
