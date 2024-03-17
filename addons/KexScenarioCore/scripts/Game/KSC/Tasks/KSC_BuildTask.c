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
	
	[Attribute(defvalue: "100", desc: "Radius within the specified structure has to be built", category: "Build")]
	protected float m_fBuildRadius;
	
	[Attribute(defvalue: "10", desc: "Timeout in seconds for checking that the built has been completed", category: "Build")]
	protected float m_fHandlerTimeout;
	protected float m_fHandlerTimer = 0;
	
	protected IEntity m_pTargetStructure;
	
	//------------------------------------------------------------------------------------------------
	//! Enable FRAME event on server and set m_sFormatParam2 to target structure
	//! m_sFormatParam1 is already reserved for the location name
	void KSC_BuildTask(IEntitySource src, IEntity parent)
	{
		if (Replication.IsServer())
			SetEventMask(EntityEvent.FRAME);
		
		m_sFormatParam2 = GetTargetStructureName();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radius within the structure has to be built
	void SetBuildRadius(float radius)
	{
		m_fBuildRadius = radius;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when the target structure has been placed
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fHandlerTimer += timeSlice;
		
		if (m_fHandlerTimer < m_fHandlerTimeout)
			return;
		
		m_fHandlerTimer = 0;
		
		if (!m_pSupportEntity)
			return;
		
		KSC_CircleArea area = KSC_CircleArea(GetOrigin(), m_fBuildRadius);
		array<IEntity> matches = {};
		KSC_WorldTools.GetEntitiesByPrefabName(matches, m_sPrefabNameToBuild, area, true);
		
		if (!matches.IsEmpty())
			m_pSupportEntity.FinishTask(this);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get the name of the target structure to be built by temporarily spawning it
	LocalizedString GetTargetStructureName()
	{
		LocalizedString name = string.Empty;
		
		IEntity tmp = GetGame().SpawnEntityPrefab(Resource.Load(m_sPrefabNameToBuild));
		if (!tmp)
			return name;
		
		SCR_EditableEntityComponent edit = SCR_EditableEntityComponent.Cast(tmp.FindComponent(SCR_EditableEntityComponent));
		if (edit)
			name = edit.GetDisplayName();
		
		SCR_EntityHelper.DeleteEntityAndChildren(tmp);
		return name;
	}
}