//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTaskClass : KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTask : KSC_BaseTask
{
	[Attribute(desc: "Prefab name of the trigger")]
	protected ResourceName m_sTriggerPrefabName;
	protected SCR_BaseTriggerEntity m_pTrigger;
	
	//------------------------------------------------------------------------------------------------
	void KSC_AreaTriggerTask(IEntitySource src, IEntity parent)
	{
		m_pTrigger = SCR_BaseTriggerEntity.Cast(KSC_GameTools.SpawnPrefab(m_sTriggerPrefabName, GetOrigin()));
		
		if (!m_pTrigger)
			return;
		
		m_pTrigger.GetOnActivate().Insert(OnTriggerActivate);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnTriggerActivate()
	{
		if (m_pSupportEntity)
			m_pSupportEntity.FinishTask(this);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void CleanUp()
	{
		if (m_pTrigger)
			SCR_EntityHelper.DeleteEntityAndChildren(m_pTrigger);
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_BaseTriggerEntity GetTrigger()
	{
		return m_pTrigger;
	}
}