//------------------------------------------------------------------------------------------------
//! Add invoker for disarming
modded class SCR_PressureTriggerComponent : SCR_BaseTriggerComponent
{
	protected ref ScriptInvoker m_KSC_OnDisarmTrigger;
	
	//------------------------------------------------------------------------------------------------
	override void DisarmTrigger()
	{
		super.DisarmTrigger();
		
		if (m_KSC_OnDisarmTrigger)
			m_KSC_OnDisarmTrigger.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker KSC_GetOnDisarmTrigger()
	{
		if (!m_KSC_OnDisarmTrigger)
			m_KSC_OnDisarmTrigger = new ScriptInvoker();
		
		return m_KSC_OnDisarmTrigger;
	}
}
