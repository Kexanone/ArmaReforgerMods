//------------------------------------------------------------------------------------------------
class KSC_BuildTaskSupportEntityClass : KSC_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_SubjectBaseTask
class KSC_BuildTaskSupportEntity : KSC_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, ResourceName prefabNameToBuild, string formatParam2 = "", string formatParam3 = "")
	{
		Resource res = Resource.Load(prefabNameToBuild);
		if (!res.IsValid())
			return null;
		
		KSC_BuildTask task = KSC_BuildTask.Cast(super.CreateTask(targetFaction, pos, KSC_BaseContainerTools.GetDisplayName(res.GetResource()), formatParam2, formatParam3));
		task.SetTargetStructurePrefabName(prefabNameToBuild);
		return task;
	}
}
