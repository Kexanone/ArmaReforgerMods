//------------------------------------------------------------------------------------------------
class KSC_DemineEffectModuleTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when the subject has been demined
class KSC_DemineEffectModuleTask : KSC_SubjectBaseTask
{
	protected int m_iMineCount = 0;
	
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		KSC_DemineEffectsModuleComponent effect = KSC_DemineEffectsModuleComponent.Cast(m_pSubject.FindComponent(KSC_DemineEffectsModuleComponent));
		if (!effect)
			return;
		
		effect.GetOnAllDemined().Insert(OnAllDemined);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void DetachSubjectHandlers()
	{
		super.DetachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		KSC_DemineEffectsModuleComponent effect = KSC_DemineEffectsModuleComponent.Cast(m_pSubject.FindComponent(KSC_DemineEffectsModuleComponent));
		if (!effect)
			return;
		
		effect.GetOnAllDemined().Remove(OnAllDemined);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnAllDemined(IEntity subject)
	{
		if (s_pTaskSystem)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
}
