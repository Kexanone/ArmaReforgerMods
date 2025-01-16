//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTaskClass : KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTask : KSC_BaseTask
{
	protected SCR_BaseTriggerEntity m_pTrigger;
	
	//------------------------------------------------------------------------------------------------
	protected void OnTriggerActivate()
	{
		if (m_pTrigger)
			m_pTrigger.GetOnActivate().Remove(OnTriggerActivate);
		
		if (m_pSupportEntity)
			m_pSupportEntity.FinishTask(this);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void CleanUp()
	{
		super.CleanUp();
		
		if (m_pTrigger)
			m_pTrigger.GetOnActivate().Remove(OnTriggerActivate);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTrigger(notnull SCR_BaseTriggerEntity trigger)
	{
		if (m_pTrigger)
			m_pTrigger.GetOnActivate().Remove(OnTriggerActivate);
		
		m_pTrigger = trigger;
		m_pTrigger.GetOnActivate().Insert(OnTriggerActivate);
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_BaseTriggerEntity GetTrigger()
	{
		return m_pTrigger;
	}
}