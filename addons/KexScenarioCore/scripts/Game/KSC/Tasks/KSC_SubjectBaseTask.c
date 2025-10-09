//------------------------------------------------------------------------------------------------
class KSC_SubjectBaseTaskClass: KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! Base class for tasks that are associated with an object
class KSC_SubjectBaseTask : KSC_BaseTask
{
	[Attribute(defvalue: "30", desc: "Timeout in seconds for updating the task's position")]
	protected float m_fPositionUpdateTimeout;
	protected float m_fPositionUpdateTimer = 0;
	
	protected IEntity m_pSubject;
	
	
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, IEntity subject, array<LocalizedString> formatParams = null)
	{
		array<LocalizedString> formatParams_BS = {};
		if (formatParams)
			formatParams_BS = formatParams;
		
		formatParams_BS.InsertAt(GetSubjectName(subject), 0);
		super.SetParams(targetFaction, formatParams_BS);
		m_pSubject = subject;
		AttachSubjectHandlers();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get the subject's name
	LocalizedString GetSubjectName(IEntity subject)
	{
		SCR_EditableEntityComponent edit = SCR_EditableEntityComponent.Cast(subject.FindComponent(SCR_EditableEntityComponent));
		if (!edit)
			return string.Empty;
		
		return edit.GetDisplayName();
		
	}
	
	//------------------------------------------------------------------------------------------------
	//! Enable FRAME event on server
	void KSC_SubjectBaseTask(IEntitySource src, IEntity parent)
	{
		if (Replication.IsServer())
			SetEventMask(EntityEvent.FRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Detach subject handlers
	override protected void CleanUp()
	{
		super.CleanUp();
		
		if (m_pSubject)
			DetachSubjectHandlers();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Put here handlers change the task's state based on the subject's state
	protected void AttachSubjectHandlers()
	{
		SCR_GarbageSystem garbageSystem = SCR_GarbageSystem.GetByEntityWorld(m_pSubject);
		if (garbageSystem)
			garbageSystem.UpdateBlacklist(m_pSubject, true);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Remove here the handlers that got attached in AttachSubjectHandlers
	protected void DetachSubjectHandlers()
	{		SCR_GarbageSystem garbageSystem = SCR_GarbageSystem.GetByEntityWorld(m_pSubject);
		if (garbageSystem)
			garbageSystem.UpdateBlacklist(m_pSubject, false);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Update task position to subject position
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fPositionUpdateTimer += timeSlice;
		
		if (m_fPositionUpdateTimer < m_fPositionUpdateTimeout)
			return;
		
		m_fPositionUpdateTimer = 0;
		
		if (!m_pSubject)
			return;
		
		vector newPos = m_pSubject.GetOrigin();
		
		if (newPos == GetOrigin())
			return;
		
		s_pTaskSystem.MoveTask(this, newPos);
	}	
}