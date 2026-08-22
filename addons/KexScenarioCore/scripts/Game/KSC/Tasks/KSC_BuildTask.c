//------------------------------------------------------------------------------------------------
class KSC_BuildTaskClass: KSC_BaseTaskClass
{
}


//------------------------------------------------------------------------------------------------
//! This task gets completed when the specified structure has been built in a given area
class KSC_BuildTask : KSC_BaseTask
{
	[Attribute(desc: "Required labels for the structure to build", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel), category: "Build")]
	protected ref array<EEditableEntityLabel> m_aRequiredLabels;
	
	[Attribute(defvalue: "0", desc: "Radius within the specified structure has to be built. Anywhere if 0", category: "Build")]
	protected float m_fBuildRadius;
	
	//------------------------------------------------------------------------------------------------
	//! Complete task if built structure is of the target type
	protected void OnCompositionBuilt(IEntity structure)
	{
		SCR_EditableEntityComponent editableStructure = SCR_EditableEntityComponent.GetEditableEntity(structure);
		if (!editableStructure)
			return;
		
		SCR_EditableEntityUIInfo info = SCR_EditableEntityUIInfo.Cast(editableStructure.GetInfo());
		if (!info)
			return;
		
		foreach (EEditableEntityLabel label : m_aRequiredLabels)
		{
			if (!info.HasEntityLabel(label))
				return;
		}
		
		SCR_CampaignBuildingCompositionComponent.KSC_GetOnCompositionSpawnedServer().Remove(OnCompositionBuilt);
		
		if (s_pTaskSystem)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, array<EEditableEntityLabel> requiredLabels = null, float radius = -1, array<LocalizedString> formatParams = null)
	{
		if (requiredLabels)
			m_aRequiredLabels = requiredLabels;
		
		if (radius > 0)
			m_fBuildRadius = radius;
	
		array<LocalizedString> extendedFormatParams = {};
		if (formatParams)
			extendedFormatParams = formatParams;
		
		extendedFormatParams.InsertAt(KSC_WorldTools.GetLocationName(GetOrigin()), 0);
		super.SetParams(targetFaction, extendedFormatParams);
		SCR_CampaignBuildingCompositionComponent.KSC_GetOnCompositionSpawnedServer().Insert(OnCompositionBuilt);
	}
}
