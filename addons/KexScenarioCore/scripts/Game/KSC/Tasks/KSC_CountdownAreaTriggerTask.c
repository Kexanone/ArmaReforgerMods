//------------------------------------------------------------------------------------------------
class KSC_CountdownAreaTriggerTaskClass : KSC_AreaTriggerTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_CountdownAreaTriggerTask : KSC_AreaTriggerTask
{
	//------------------------------------------------------------------------------------------------
	override protected void OnTriggerActivate(IEntity ent)
	{
		if (m_pTrigger != ent)
			return;
		
		if (m_pTrigger)
			m_pTrigger.GetOnActivate().Remove(OnTriggerActivate);
		
		if (s_pTaskSystem)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
}
