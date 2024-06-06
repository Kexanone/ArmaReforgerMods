//------------------------------------------------------------------------------------------------
class KSC_BaseTaskClass: SCR_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! Implements pop-up notifications, script invokers, title and description formatting.
class KSC_BaseTask : SCR_BaseTask
{
	protected string m_sFormatParam1 = string.Empty;
	protected string m_sFormatParam2 = string.Empty;
	protected string m_sFormatParam3 = string.Empty;
	protected ref ScriptInvoker m_OnStateChanged;
	protected ref ScriptInvoker m_OnCleanUp;
	protected KSC_BaseTaskSupportEntity m_pSupportEntity;
	
	
	//------------------------------------------------------------------------------------------------
	void SetSupportEntity(KSC_BaseTaskSupportEntity supportEntity)
	{
		m_pSupportEntity = supportEntity;
	}
	
	//------------------------------------------------------------------------------------------------
	KSC_BaseTaskSupportEntity GetSupportEntity()
	{
		return m_pSupportEntity;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task's state has been changed
	//! Based on SCR_EditorTask.PopUpNotification
	protected void PopUpNotification(string prefix)
	{
		//--- Get player faction (prioritize respawn faction, because it's defined even when player is waiting for respawn)
		Faction playerFaction;
		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (factionManager)
			playerFaction = factionManager.GetLocalPlayerFaction();		
				
		if (!playerFaction)
			playerFaction = SCR_PlayerController.GetLocalMainEntityFaction();
		
		//--- Show notification when player is assigned, of the same faction, or has unlimited editor (i.e., is Game Master)
		if (IsAssignedToLocalPlayer() || playerFaction == GetTargetFaction())
		{
			//--- SCR_PopUpNotification.GetInstance() is never null, as it creates the instance if it doesn't exist yet
			SCR_PopUpNotification.GetInstance().PopupMsg(prefix + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: m_sFormatParam1, param2: m_sFormatParam2, param3: m_sFormatParam3, sound: SCR_SoundEvent.TASK_SUCCEED);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task is (un)assigned
	//! Based on SCR_EditorTask.ShowPopUpNotification
	override protected void ShowPopUpNotification(string subtitle)
	{
		SCR_PopUpNotification.GetInstance().PopupMsg(GetTitle(), text2: subtitle, param1: m_sFormatParam1, param2: m_sFormatParam2, param3: m_sFormatParam3);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Delete task when it has been completed or cancelled and call script invoker
	//! Based on SCR_EditorTask.OnStateChanged
	protected override void OnStateChanged(SCR_TaskState previousState, SCR_TaskState newState)
	{
		if (m_OnStateChanged)
			m_OnStateChanged.Invoke(this, previousState, newState);
		
		// Delete the task once it's finished
		if (newState == SCR_TaskState.FINISHED || newState == SCR_TaskState.CANCELLED)
		{
			GetTaskManager().DeleteTask(this);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task is created
	//! Based on SCR_EditorTask.Create
	override void Create(bool showMsg = true)
	{
		super.Create(showMsg);
		
		if (showMsg)
			PopUpNotification(TASK_AVAILABLE_TEXT);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task is finished
	//! Based on SCR_EditorTask.Finish
	override void Finish(bool showMsg = true)
	{
		super.Finish(showMsg);
		
		if (showMsg)
			PopUpNotification(TASK_COMPLETED_TEXT);
		
		CleanUp();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task has failed
	//! Based on SCR_EditorTask.Fail
	override void Fail(bool showMsg = true)
	{
		super.Fail(showMsg);
		
		if (showMsg)
			PopUpNotification(TASK_FAILED_TEXT);
		
		CleanUp();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show pop-up notification when task is canceled
	//! Based on SCR_EditorTask.Cancel
	override void Cancel(bool showMsg = true)
	{
		super.Cancel(showMsg);
		
		if (showMsg)
			PopUpNotification(TASK_CANCELLED_TEXT);
		
		CleanUp();
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
	void SetFormatParams(string param1 = string.Empty, string param2 = string.Empty, string param3 = string.Empty)
	{
		m_sFormatParam1 = param1;
		m_sFormatParam2 = param2;
		m_sFormatParam3 = param3;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Format task title
	override void SetTitleWidgetText(notnull TextWidget textWidget, string taskText)
	{
		textWidget.SetTextFormat(taskText, m_sFormatParam1, m_sFormatParam2, m_sFormatParam3);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Format task description
	override void SetDescriptionWidgetText(notnull TextWidget textWidget, string taskText)
	{
		textWidget.SetTextFormat(taskText, m_sFormatParam1, m_sFormatParam2, m_sFormatParam3);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Serialize params for formatting task title and description
	override void Serialize(ScriptBitWriter writer)
	{
		super.Serialize(writer);
		writer.WriteString(m_sFormatParam1);
		writer.WriteString(m_sFormatParam2);
		writer.WriteString(m_sFormatParam3);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Dserialize params for formatting task title and description
	override void Deserialize(ScriptBitReader reader)
	{
		super.Deserialize(reader);
		reader.ReadString(m_sFormatParam1);
		reader.ReadString(m_sFormatParam2);
		reader.ReadString(m_sFormatParam3);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_BaseTask()
	{
		if (Replication.IsServer())
			CleanUp();
	}
}
