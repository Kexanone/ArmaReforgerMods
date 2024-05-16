//------------------------------------------------------------------------------------------------
class KSC_BaseTaskSupportEntityClass: SCR_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask
[BaseContainerProps()]
class KSC_BaseTaskSupportEntity : SCR_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	//! Set the support entity that created the task
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(super.CreateTask());
		
		task.SetSupportEntity(this);
		int taskID = task.GetTaskID();
		int factionIdx = GetGame().GetFactionManager().GetFactionIndex(targetFaction);
		CreateTaskBroadcast(taskID, factionIdx, pos, formatParam1, formatParam2, formatParam3);
		Rpc(CreateTaskBroadcast, taskID, factionIdx, pos, formatParam1, formatParam2, formatParam3);
		return task;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set the task's position and format title and description
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void CreateTaskBroadcast(int taskID, int factionIdx, vector pos, string param1, string param2, string param3)
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		task.SetTargetFaction(GetGame().GetFactionManager().GetFactionByIndex(factionIdx));
		task.SetOrigin(pos);
		task.SetFormatParams(param1, param2, param3);
		task.Create();
	}
	
	//------------------------------------------------------------------------------------------------
	LocalizedString GetTaskName()
	{
		Resource taskRes = Resource.Load(GetResourceName());
		if (!taskRes.IsValid())
			return "";
		
		BaseContainer taskContainer = taskRes.GetResource();
		if (!taskContainer)
			return "";
		
		string name;
		taskContainer.Get("m_sName", name);
		return name;
	}
}