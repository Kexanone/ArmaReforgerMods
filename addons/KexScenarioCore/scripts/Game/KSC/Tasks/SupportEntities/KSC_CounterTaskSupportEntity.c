//------------------------------------------------------------------------------------------------
class KSC_CounterTaskSupportEntityClass : KSC_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_CounterTask
class KSC_CounterTaskSupportEntity : KSC_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, int targetCount, string formatParam3 = "")
	{
		KSC_CounterTask task = KSC_CounterTask.Cast(super.CreateTask(targetFaction, pos, "0", targetCount.ToString(), formatParam3));
		task.SetTargetCount(targetCount);
		return task;
	}
}