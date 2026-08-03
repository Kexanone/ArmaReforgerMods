//------------------------------------------------------------------------------------------------
class KSC_DemineEffectsModuleComponentClass : SCR_EffectsModuleComponentClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_DemineEffectsModuleComponent : SCR_EffectsModuleComponent
{
	protected int m_iArmedCount = 0;
	protected ref ScriptInvoker m_OnAllDemined;
	
	//------------------------------------------------------------------------------------------------
	override protected void PostEntitySpawned(notnull SCR_EffectsModuleChildComponent spawnedEntity, vector targetPosition)
	{
		super.PostEntitySpawned(spawnedEntity, targetPosition);
		
		if (!spawnedEntity)
			return;
		
		IEntity spawnedOwner = spawnedEntity.GetOwner();
		if (!spawnedOwner)
			return;
		
		SCR_PressureTriggerComponent trigger = SCR_PressureTriggerComponent.Cast(spawnedOwner.FindComponent(SCR_PressureTriggerComponent));
		if (!trigger)
			return;
			
		trigger.KSC_GetOnDoTrigger().Insert(HandleDemining);
		trigger.KSC_GetOnDisarmTrigger().Insert(HandleDemining);
		m_iArmedCount++;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void HandleDemining(SCR_PressureTriggerComponent trigger)
	{
		if (trigger)
		{
			trigger.KSC_GetOnDoTrigger().Remove(HandleDemining);
			trigger.KSC_GetOnDisarmTrigger().Remove(HandleDemining);
		}
		
		m_iArmedCount--;
		
		if (m_iArmedCount > 0)
			return;
		
		if (m_OnAllDemined)
			m_OnAllDemined.Invoke(GetOwner());
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnAllDemined()
	{
		if (!m_OnAllDemined)
			m_OnAllDemined = new ScriptInvoker();
		
		return m_OnAllDemined;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnDelete(IEntity owner)
	{
		super.OnDelete(owner);
		
		if (m_OnAllDemined)
			m_OnAllDemined.Invoke(owner);
	}
}
