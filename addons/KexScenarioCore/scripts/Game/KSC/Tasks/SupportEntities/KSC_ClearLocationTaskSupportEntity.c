//------------------------------------------------------------------------------------------------
class KSC_ClearAreaTaskSupportEntityClass : KSC_AreaTriggerTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask that are associated with a location
class KSC_ClearAreaTaskSupportEntity : KSC_AreaTriggerTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	//! Same as KSC_BaseTaskSupportEntity, but formatParam1 gets ignored and replaced by the location name
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, float radius, float friendlyRatioThreshold, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		KSC_AreaTriggerTask task = KSC_AreaTriggerTask.Cast(super.CreateTask(targetFaction, pos, radius, formatParam2, formatParam2, formatParam3));
		
		KSC_FactionControlTriggerEntity trigger = KSC_FactionControlTriggerEntity.Cast(task.GetTrigger());
		if (trigger)
			trigger.SetFriendlyRatioLimit(friendlyRatioThreshold);
		
		return task;
	}
}