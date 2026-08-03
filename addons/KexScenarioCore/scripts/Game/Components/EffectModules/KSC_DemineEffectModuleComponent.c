//------------------------------------------------------------------------------------------------
class KSC_DemineEffectsModuleComponentClass : SCR_EffectsModuleComponentClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_DemineEffectsModuleComponent : SCR_EffectsModuleComponent
{
	protected ref array<ref KSC_DeminingEffectModuleComponent_MineEventsWrapper> m_MineWrappers = {};
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
		
		m_MineWrappers.Insert(new KSC_DeminingEffectModuleComponent_MineEventsWrapper(this, spawnedOwner));
		m_iArmedCount++;
	}
	
	//------------------------------------------------------------------------------------------------
	[Friend(KSC_DeminingEffectModuleComponent_MineEventsWrapper)]
	protected void HandleDemining(KSC_DeminingEffectModuleComponent_MineEventsWrapper wrapper)
	{
		m_MineWrappers.RemoveItem(wrapper);
		m_iArmedCount--;
		
		if (m_iArmedCount <= 0 && m_OnAllDemined)
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

//------------------------------------------------------------------------------------------------
//! Events wrapper for mine that calls back parent on demining
class KSC_DeminingEffectModuleComponent_MineEventsWrapper
{
	protected KSC_DemineEffectsModuleComponent m_Parent;
	protected IEntity m_Mine;
	
	//------------------------------------------------------------------------------------------------
	void KSC_DeminingEffectModuleComponent_MineEventsWrapper(KSC_DemineEffectsModuleComponent parent, IEntity mine)
	{
		m_Parent = parent;
		m_Mine = mine;
		
		SCR_PressureTriggerComponent trigger = SCR_PressureTriggerComponent.Cast(mine.FindComponent(SCR_PressureTriggerComponent));
		if (trigger)
			trigger.KSC_GetOnDisarmTrigger().Insert(HandleDemining);
		
		SCR_MineDamageManager damageManager = SCR_MineDamageManager.Cast(mine.FindComponent(SCR_MineDamageManager));
		if (damageManager)
			damageManager.GetOnDamageStateChanged().Insert(HandleDamageStateChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void HandleDemining(SCR_PressureTriggerComponent trigger)
	{
		m_Parent.HandleDemining(this);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void HandleDamageStateChanged(EDamageState newState)
	{
		if (newState != EDamageState.DESTROYED)
			return;
		
		m_Parent.HandleDemining(this);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_DeminingEffectModuleComponent_MineEventsWrapper()
	{
		if (!m_Mine)
			return;
		
		SCR_PressureTriggerComponent trigger = SCR_PressureTriggerComponent.Cast(m_Mine.FindComponent(SCR_PressureTriggerComponent));
		if (trigger)
			trigger.KSC_GetOnDisarmTrigger().Insert(HandleDemining);
		
		SCR_MineDamageManager damageManager = SCR_MineDamageManager.Cast(m_Mine.FindComponent(SCR_MineDamageManager));
		if (damageManager)
			damageManager.GetOnDamageStateChanged().Insert(HandleDamageStateChanged);
	}
}
