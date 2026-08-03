//------------------------------------------------------------------------------------------------
//! Add invoker for disarming
modded class SCR_PressureTriggerComponent : SCR_BaseTriggerComponent
{
	protected ref ScriptInvoker m_KSC_OnDoTrigger;
	protected ref ScriptInvoker m_KSC_OnDisarmTrigger;
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	override void RPC_DoTrigger()
	{
		super.RPC_DoTrigger();
		
		if (m_KSC_OnDoTrigger)
			m_KSC_OnDoTrigger.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	override void DisarmTrigger()
	{
		super.DisarmTrigger();
		
		if (m_KSC_OnDisarmTrigger)
			m_KSC_OnDisarmTrigger.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker KSC_GetOnDoTrigger()
	{
		if (!m_KSC_OnDoTrigger)
			m_KSC_OnDoTrigger = new ScriptInvoker();
		
		return m_KSC_OnDoTrigger;
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker KSC_GetOnDisarmTrigger()
	{
		if (!m_KSC_OnDisarmTrigger)
			m_KSC_OnDisarmTrigger = new ScriptInvoker();
		
		return m_KSC_OnDisarmTrigger;
	}
}
