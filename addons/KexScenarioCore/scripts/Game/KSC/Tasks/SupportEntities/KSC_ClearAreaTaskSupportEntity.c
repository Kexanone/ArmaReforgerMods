//------------------------------------------------------------------------------------------------
class KSC_ClearAreaTaskSupportEntityClass : KSC_AreaTriggerTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask that are associated with a location
class KSC_ClearAreaTaskSupportEntity : KSC_AreaTriggerTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, float radius = 50, float friendlyRatioThreshold = 0.528, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		KSC_AreaTriggerTask task = KSC_AreaTriggerTask.Cast(super.CreateTask(targetFaction, pos, radius, formatParam1, formatParam2, formatParam3));
		KSC_FactionDominanceTriggerEntity trigger = KSC_FactionDominanceTriggerEntity.Cast(task.GetTrigger());
		trigger.SetOwnerFaction(targetFaction);
		trigger.SetFriendlyRatioLimit(friendlyRatioThreshold);
		return task;
	}
}
