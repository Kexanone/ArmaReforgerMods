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
	void SetParams(Faction targetFaction, ResourceName prefabnameToBuild = "", float radius = -1, array<LocalizedString> formatParams = null)
	{
		if (!prefabnameToBuild.IsEmpty())
			m_sPrefabNameToBuild = prefabnameToBuild;
		
		if (radius > 0)
			m_fBuildRadius = radius;
	
		array<LocalizedString> extendedFormatParams = {};
		if (formatParams)
			extendedFormatParams = formatParams;
		
		Resource res = Resource.Load(m_sPrefabNameToBuild);
		if (res.IsValid())
			extendedFormatParams.Insert(KSC_BaseContainerTools.GetDisplayName(res.GetResource()));
		
		super.SetParams(targetFaction, extendedFormatParams);
		SCR_CampaignBuildingCompositionComponent.KSC_GetOnCompositionSpawnedServer().Insert(OnCompositionBuilt);
	}
}
