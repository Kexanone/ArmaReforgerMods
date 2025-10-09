//------------------------------------------------------------------------------------------------
class KSC_KillTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when the subject has been killed
class KSC_KillTask : KSC_SubjectBaseTask
{
	protected SCR_DamageManagerComponent m_pDamageManager;
	
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		m_pDamageManager = SCR_DamageManagerComponent.Cast(m_pSubject.FindComponent(SCR_DamageManagerComponent));
		IEntity child = m_pSubject.GetChildren();
		
		while (!m_pDamageManager && child)
		{
			m_pDamageManager = SCR_DamageManagerComponent.Cast(child.FindComponent(SCR_DamageManagerComponent));			
			child = child.GetSibling();
		};
		
		if (m_pDamageManager)
			m_pDamageManager.GetOnDamageStateChanged().Insert(OnObjectDamage);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void DetachSubjectHandlers()
	{
		super.DetachSubjectHandlers();
		
		if (m_pDamageManager)
		 	m_pDamageManager.GetOnDamageStateChanged().Remove(OnObjectDamage);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when subject got killed
	void OnObjectDamage(EDamageState state)
	{
		if (state != EDamageState.DESTROYED || !m_pSubject)
			return;
		
		s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);			
	}
}