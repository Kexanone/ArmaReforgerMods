//------------------------------------------------------------------------------------------------
class KSC_SuperTaskClass: KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when all child tasks are completed
class KSC_SuperTask : KSC_BaseTask
{
	protected ref array<KSC_BaseTask> m_aChildren;
	protected int m_iFinishedChildrenCounter = 0;
	
	//------------------------------------------------------------------------------------------------
	//! Add a child task and attach on finished handler
	void AddChildTask(KSC_BaseTask child)
	{
		m_aChildren.Insert(child);
		child.GetOnStateChanged().Insert(OnChildStateChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when all children are finished or cancelled
	void OnChildStateChanged(KSC_BaseTask child, SCR_TaskState previousState, SCR_TaskState newState)
	{
		if (newState != SCR_TaskState.FINISHED && newState != SCR_TaskState.CANCELLED)
			return;
		
		m_iFinishedChildrenCounter++;
		if (m_iFinishedChildrenCounter < m_aChildren.Count())
			return;
		
		m_pSupportEntity.FinishTask(this);
	}
}