//------------------------------------------------------------------------------------------------
class KSC_BaseTaskSupportEntityClass: SCR_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask
class KSC_BaseTaskSupportEntity : SCR_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	//! Set the support entity that created the task
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(super.CreateTask());
		task.SetTargetFaction(targetFaction);
		task.SetSupportEntity(this);
		int taskID = task.GetTaskID();
		Rpc(CreateTaskBroadcast, taskID, pos, formatParam1, formatParam2, formatParam3);
		CreateTaskBroadcast(taskID, pos, formatParam1, formatParam2, formatParam3);
		return task;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set the task's position and format title and description
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void CreateTaskBroadcast(int taskID, vector pos, string param1, string param2, string param3)
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		task.SetOrigin(pos);
		task.SetFormatParams(param1, param2, param3);
		task.Create();
	}
}