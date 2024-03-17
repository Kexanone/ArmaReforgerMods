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
	//! Enable FRAME event on server
	void KSC_SubjectBaseTask(IEntitySource src, IEntity parent)
	{
		if (Replication.IsServer())
			SetEventMask(EntityEvent.FRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set subject and attach handlers
	void SetSubject(IEntity subject)
	{
		if (m_pSubject)
			DetachSubjectHandlers();
		
		m_pSubject = subject;
		AttachSubjectHandlers();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Detach subject handlers
	override void Finish(bool showMsg = true)
	{
		super.Finish(showMsg);
		
		if (m_pSubject)
			DetachSubjectHandlers();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Put here handlers change the task's state based on the subject's state
	protected void AttachSubjectHandlers()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	//! Remove here the handlers that got attached in AttachSubjectHandlers
	protected void DetachSubjectHandlers()
	{
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
		
		m_pSupportEntity.MoveTask(newPos, GetTaskID());
	}	
}