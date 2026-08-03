//------------------------------------------------------------------------------------------------
//! Fix call invoker of SCR_DamageManagerComponent
modded class SCR_MineDamageManager : SCR_DamageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	override event protected void OnDamageStateChanged(EDamageState newState, EDamageState previousDamageState, bool isJIP)
	{
		super.OnDamageStateChanged(newState, previousDamageState, isJIP);
		
		if (m_iDamageManagerDataIndex != -1)
		{
			ScriptInvoker invoker = s_aDamageManagerData[m_iDamageManagerDataIndex].GetOnDamageStateChanged(false);
			if (invoker)
				invoker.Invoke(newState);
		}
	}
}
