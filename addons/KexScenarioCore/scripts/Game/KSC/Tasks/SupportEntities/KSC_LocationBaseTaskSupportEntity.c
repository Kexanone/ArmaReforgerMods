//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTaskSupportEntityClass: KSC_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask that are associated with a location
class KSC_AreaTriggerTaskSupportEntity : KSC_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	//! Same as KSC_BaseTaskSupportEntity, but formatParam1 gets ignored and replaced by the location name
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, float radius, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		KSC_AreaTriggerTask task = KSC_AreaTriggerTask.Cast(super.CreateTask(targetFaction, pos, formatParam2, formatParam2, formatParam3));
		SCR_BaseTriggerEntity trigger = task.GetTrigger();
		trigger.SetSphereRadius(radius);
		return task;
	}
}