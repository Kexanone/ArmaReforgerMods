//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTaskClass : KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTask : KSC_BaseTask
{
	[Attribute(desc: "Prefab name of the trigger", uiwidget: UIWidgets.ResourceNamePicker, params: "et")]
	protected ResourceName m_sTriggerPrefabName;
	
	// Start of queries has to be delayed, otherwise the trigger gets completed before the AI spawns
	[Attribute(defvalue: "10", desc: "Delay in seconds for the trigger to start after spawning")]
	protected float m_fTriggerStartDelay
	
	protected SCR_BaseTriggerEntity m_pTrigger;
	
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, float radius = 50, array<LocalizedString> formatParams = null)
	{
		array<LocalizedString> formatParams_BS = {};
		if (formatParams)
			formatParams_BS = formatParams;
		
		formatParams_BS.InsertAt(KSC_WorldTools.GetLocationName(GetOrigin()), 0);
		super.SetParams(targetFaction, formatParams_BS);
		
		Resource triggerRes = Resource.Load(m_sTriggerPrefabName);
		if (!triggerRes.IsValid())
			return;
		
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.Parent = this;
		m_pTrigger  = SCR_BaseTriggerEntity.Cast(GetGame().SpawnEntityPrefabLocal(triggerRes, GetWorld(), spawnParams));
		m_pTrigger.GetOnActivate().Insert(OnTriggerActivate);
		m_pTrigger.SetSphereRadius(radius);
		m_pTrigger.EnablePeriodicQueries(false);
		GetGame().GetCallqueue().CallLater(m_pTrigger.EnablePeriodicQueries, m_fTriggerStartDelay * 1000, false, true);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void CleanUp()
	{
		super.CleanUp();
		
		if (m_pTrigger)
			SCR_EntityHelper.DeleteEntityAndChildren(m_pTrigger);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnTriggerActivate()
	{
		if (m_pTrigger)
			m_pTrigger.GetOnActivate().Remove(OnTriggerActivate);
		
		if (s_pTaskSystem)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
}

