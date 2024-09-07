//------------------------------------------------------------------------------------------------
class KSC_BaseTaskSupportEntityClass: SCR_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask
[BaseContainerProps()]
class KSC_BaseTaskSupportEntity : SCR_BaseTaskSupportEntity
{
	[Attribute(defvalue: "false", desc: "True if it should create primary tasks")]
	protected bool m_bIsPrimaryTask;
	
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
		task.SetIsPriority(m_bIsPrimaryTask);
		task.Create();
	}
	
	//------------------------------------------------------------------------------------------------
	void PopUpNotification(KSC_BaseTask task, string prefix)
	{
		int taskID = task.GetTaskID();
		RpcDo_PopUpNotificationBroadcast(taskID, prefix);
		Rpc(RpcDo_PopUpNotificationBroadcast, taskID, prefix);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_PopUpNotificationBroadcast(int taskID, string prefix)
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		task.PopUpNotification(prefix);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetFormatParams(KSC_BaseTask task, string param1 = "", string param2 = "", string param3 = "")
	{
		int taskID = task.GetTaskID();
		RpcDo_SetFormatParamsBroadcast(taskID, param1, param2, param3);
		Rpc(RpcDo_SetFormatParamsBroadcast, taskID, param1, param2, param3);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_SetFormatParamsBroadcast(int taskID, string param1, string param2, string param3)
	{
		KSC_BaseTask task = KSC_BaseTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		task.SetFormatParams(param1, param2, param3);
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