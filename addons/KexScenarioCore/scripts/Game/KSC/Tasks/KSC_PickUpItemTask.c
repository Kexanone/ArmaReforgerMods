//------------------------------------------------------------------------------------------------
class KSC_PickUpItemTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed with the subject has been picked up
class KSC_PickUpItemTask : KSC_SubjectBaseTask
{
	protected InventoryItemComponent m_pItemComponent;
	
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		m_pItemComponent = InventoryItemComponent.Cast(m_pSubject.FindComponent(InventoryItemComponent));
		if (!m_pItemComponent)
			return;
		
		m_pItemComponent.m_OnParentSlotChangedInvoker.Insert(OnParentSlotChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void DetachSubjectHandlers()
	{
		super.DetachSubjectHandlers();
		
		if (!m_pItemComponent)
			return;
		
		m_pItemComponent.m_OnParentSlotChangedInvoker.Remove(OnParentSlotChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when the subject has been moved to a new slot
	void OnParentSlotChanged(EDamageState state)
	{
		m_pSupportEntity.FinishTask(this);				
	}
}