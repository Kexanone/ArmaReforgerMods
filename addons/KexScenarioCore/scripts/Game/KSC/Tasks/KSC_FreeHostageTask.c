//------------------------------------------------------------------------------------------------
class KSC_FreeHostageTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when the subject has been released from captivity
class KSC_FreeHostageTask : KSC_SubjectBaseTask
{
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(m_pSubject);
		if (!char)
			return;
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(char.GetCompartmentAccessComponent());
		if (compartmentAccess)
			compartmentAccess.GetOnCompartmentLeft().Insert(OnCompartmentLeft);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void DetachSubjectHandlers()
	{
		super.DetachSubjectHandlers();
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(m_pSubject);
		if (!char)
			return;
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(char.GetCompartmentAccessComponent());
		if (compartmentAccess)
			compartmentAccess.GetOnCompartmentLeft().Remove(OnCompartmentLeft);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when the subject has been released
	void OnCompartmentLeft(IEntity vehicle, BaseCompartmentManagerComponent manager, int mgrID, int slotID, bool move)
	{
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(m_pSubject);
		if (!char)
			return;
		
		SCR_CharacterControllerComponent charController = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (!charController)
			return;
		
		if (charController.ACE_Captives_IsCaptive())
			return;
		
		if (charController.GetLifeState() == ECharacterLifeState.DEAD)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.FAILED);
		else
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
}
