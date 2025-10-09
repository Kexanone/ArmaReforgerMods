/*****
//------------------------------------------------------------------------------------------------
class KSC_BuildTaskClass: KSC_BaseTaskClass
{
}


//------------------------------------------------------------------------------------------------
//! This task gets completed when the specified structure has been built in a given area
class KSC_BuildTask : KSC_BaseTask
{
	[Attribute(desc: "The name of the prefab to be built", category: "Build")]
	protected ResourceName m_sPrefabNameToBuild;
	
	[Attribute(defvalue: "0", desc: "Radius within the specified structure has to be built. Anywhere if 0", category: "Build")]
	protected float m_fBuildRadius;
	
	//------------------------------------------------------------------------------------------------
	void KSC_BuildTask(IEntitySource src, IEntity parent)
	{
		Resource res = Resource.Load(m_sPrefabNameToBuild);
		if (res.IsValid())
			m_sFormatParam1 = KSC_BaseContainerTools.GetDisplayName(res.GetResource());
		
		SCR_CampaignBuildingCompositionComponent.KSC_GetOnCompositionSpawnedServer().Insert(OnCompositionBuilt);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Complete task if built structure is of the target type
	protected void OnCompositionBuilt(IEntity structure)
	{
		EntityPrefabData data = structure.GetPrefabData();
		if (!data)
			return;
		
		BaseContainer container = data.GetPrefab();
		if (!container)
			return;
		
		if (!SCR_BaseContainerTools.IsKindOf(container, m_sPrefabNameToBuild))
			return;
		
		SCR_CampaignBuildingCompositionComponent.KSC_GetOnCompositionSpawnedServer().Remove(OnCompositionBuilt);
		
		if (s_pTaskSystem)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radius within the structure has to be built
	void SetBuildRadius(float radius)
	{
		m_fBuildRadius = radius;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTargetStructurePrefabName(ResourceName resName)
	{
		m_sPrefabNameToBuild = resName;
		
	}
}
*****/