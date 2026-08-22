//------------------------------------------------------------------------------------------------
class KSC_DefendCommandPostTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_DefendCommandPostTask : KSC_SubjectBaseTask
{
	protected SCR_FactionAffiliationComponent m_FactionAffiliation;
	
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		m_FactionAffiliation = SCR_FactionAffiliationComponent.Cast(m_pSubject.FindComponent(SCR_FactionAffiliationComponent));
		IEntity child = m_pSubject.GetChildren();
		
		while (!m_FactionAffiliation && child)
		{
			m_FactionAffiliation = SCR_FactionAffiliationComponent.Cast(child.FindComponent(SCR_FactionAffiliationComponent));			
			child = child.GetSibling();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void FinishTask()
	{
		if (!m_FactionAffiliation)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.CANCELLED);
		
		if (GetOwnerFactionKeys().Contains(m_FactionAffiliation.GetAffiliatedFactionKey()))
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
		else
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.FAILED);
		
		s_pTaskSystem.DeleteTask(this);
	}
}
