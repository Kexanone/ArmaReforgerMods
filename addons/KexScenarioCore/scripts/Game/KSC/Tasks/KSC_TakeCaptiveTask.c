//------------------------------------------------------------------------------------------------
class KSC_TakeCaptiveTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when the subject gets arrested
class KSC_TakeCaptiveTask : KSC_SubjectBaseTask
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
			compartmentAccess.GetOnCompartmentEntered().Insert(OnCompartmentEntered);
		
		SCR_CharacterControllerComponent charController = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (charController)
			charController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
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
			compartmentAccess.GetOnCompartmentEntered().Remove(OnCompartmentEntered);
		
		SCR_CharacterControllerComponent charController = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (charController)
			charController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when the subject has been captured
	protected void OnCompartmentEntered(IEntity vehicle, BaseCompartmentManagerComponent manager, int mgrID, int slotID, bool move)
	{
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(m_pSubject);
		if (!char)
			return;
		
		SCR_CharacterControllerComponent charController = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (!charController)
			return;
		
		if (!charController.ACE_Captives_IsCaptive())
			return;
		
		s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Fail task if subject died
	protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		if (newLifeState != ECharacterLifeState.DEAD)
			return;
		
		s_pTaskSystem.SetTaskState(this, SCR_ETaskState.FAILED);
	}
}
