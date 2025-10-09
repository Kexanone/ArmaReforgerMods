//------------------------------------------------------------------------------------------------
class KSC_BaseTaskClass: SCR_TaskClass
{
}

//------------------------------------------------------------------------------------------------
//! Implements pop-up notifications, script invokers, title and description formatting.
class KSC_BaseTask : SCR_Task
{
	protected static SCR_TaskSystem s_pTaskSystem;
	protected ref ScriptInvoker m_OnStateChanged;
	protected ref ScriptInvoker m_OnCleanUp;
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		if (!s_pTaskSystem)
			s_pTaskSystem = SCR_TaskSystem.GetInstance();		
	}
	
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, array<LocalizedString> formatParams = null)
	{
		s_pTaskSystem.AddTaskFaction(this, targetFaction.GetFactionKey());
		
		if (formatParams)
			SetFormatParams(formatParams);
		
		SetTaskState(SCR_ETaskState.CREATED);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Adapted from SCR_EditorTask
	override void SetTaskState(SCR_ETaskState state)
	{
		super.SetTaskState(state);
		
		if (m_OnStateChanged)
			m_OnStateChanged.Invoke(this, state);
		
		string text;
		bool doCleanUp = false;
		
		switch (state)
		{
			case SCR_ETaskState.CREATED:
			{
				text = SCR_TextsTaskManagerComponent.TASK_AVAILABLE_TEXT;
				break;
			}
			case SCR_ETaskState.COMPLETED:
			{
				text = SCR_TextsTaskManagerComponent.TASK_COMPLETED_TEXT;
				doCleanUp = true;
				break;
			}
			case SCR_ETaskState.FAILED:
			{
				text = SCR_TextsTaskManagerComponent.TASK_FAILED_TEXT;
				doCleanUp = true;
				break;
			}
			case SCR_ETaskState.CANCELLED:
			{
				text = SCR_TextsTaskManagerComponent.TASK_CANCELLED_TEXT;
				doCleanUp = true;
				break;
			}
			default:
				return;
		}
		
		RpcDo_PopUpNotificationBroadcast(text);
		Rpc(RpcDo_PopUpNotificationBroadcast, text);
		
		if (doCleanUp)
			CleanUp();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task's state has been changed
	//! Based on SCR_EditorTask.PopUpNotification
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_PopUpNotificationBroadcast(string prefix)
	{
		//--- Get player faction (prioritize respawn faction, because it's defined even when player is waiting for respawn)
		Faction playerFaction;
		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (factionManager)
			playerFaction = factionManager.GetLocalPlayerFaction();
				
		if (!playerFaction)
			playerFaction = SCR_PlayerController.GetLocalMainEntityFaction();
		
		string playerFactionKey;
		if (playerFaction)
			playerFactionKey = playerFaction.GetFactionKey();
		
		//--- Show notification when player is assigned, of the same faction, or has unlimited editor (i.e., is Game Master)
		if (IsTaskAssignedTo(SCR_TaskExecutor.FromLocalPlayer()) || GetOwnerFactionKeys().Contains(playerFactionKey) || (!SCR_EditorManagerEntity.IsLimitedInstance()))
		{
			array<LocalizedString> formatParams = {};
			LocalizedString taskName = GetTaskUIInfo().GetUnformattedName(formatParams);
			formatParams.Resize(4);
			//--- SCR_PopUpNotification.GetInstance() is never null, as it creates the instance if it doesn't exist yet
			SCR_PopUpNotification.GetInstance().PopupMsg(prefix + " " + taskName, prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: formatParams[0], param2: formatParams[1], param3: formatParams[2], param4: formatParams[3], sound: SCR_SoundEvent.TASK_SUCCEED);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Clean up gets triggered on authority after the task has been completed, failed or canceled
	//! and when it gets deleted.
	protected void CleanUp()
	{
		if (m_OnCleanUp)
			m_OnCleanUp.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get script invoker for when cleaning up
	ScriptInvoker GetOnCleanUp()
	{
		if (!m_OnCleanUp)
			m_OnCleanUp = new ScriptInvoker();
		
		return m_OnCleanUp;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get script invoker for when the task state has changed
	ScriptInvoker GetOnStateChanged()
	{
		if (!m_OnStateChanged)
			m_OnStateChanged = new ScriptInvoker();
		
		return m_OnStateChanged;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set params for formatting task title and description
	void SetFormatParams(array<LocalizedString> params = null)
	{
		SetTaskName(GetTaskName(), params);
		SetTaskDescription(GetTaskDescription(), params);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool RplLoad(ScriptBitReader reader)
	{
		bool result = super.RplLoad(reader);
		
		if (GetTaskState() == SCR_ETaskState.CREATED)
			RpcDo_PopUpNotificationBroadcast(SCR_TextsTaskManagerComponent.TASK_AVAILABLE_TEXT);
		
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_BaseTask()
	{
		if (Replication.IsServer())
			CleanUp();
	}
}
