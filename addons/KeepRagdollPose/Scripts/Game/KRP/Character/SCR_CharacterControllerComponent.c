//------------------------------------------------------------------------------------------------
modded class SCR_CharacterControllerComponent : CharacterControllerComponent
{
	[RplProp(onRplName: "KRP_OnToggleRagdollPose")]
	protected bool m_bKRP_RagdollPoseEnabled = false;
	
	//------------------------------------------------------------------------------------------------
	protected void KRP_OnToggleRagdollPose()
	{		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return;
		
		// Anim var has to be updated on owner and server only
		RplComponent rpl = char.GetRplComponent();
		if (!rpl || (!rpl.IsOwner() && rpl.Role() != RplRole.Authority))
			return;
		
		CharacterAnimationComponent animComponent = char.GetAnimationComponent();
		if (!animComponent)
			return;
		
		animComponent.SetVariableBool(animComponent.BindVariableBool("KRP_RagdollPose"), m_bKRP_RagdollPoseEnabled);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add/remove second chance when life state changes
	override void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState, bool isJIP)
	{
		super.OnLifeStateChanged(previousLifeState, newLifeState, isJIP);
		
		if (newLifeState != ECharacterLifeState.INCAPACITATED)
			return;
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return;
		
		RplComponent rpl = char.GetRplComponent();
		if (!rpl || rpl.IsProxy())
			return;
		
		KRP_ToggleRagdollPose(true);
	}
	
	//------------------------------------------------------------------------------------------------
	void KRP_ToggleRagdollPose(bool enabled)
	{
		m_bKRP_RagdollPoseEnabled = enabled;
		Replication.BumpMe();
		KRP_OnToggleRagdollPose();
	}
}

//------------------------------------------------------------------------------------------------
modded class ACE_Medical_RepositionUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(pOwnerEntity);
		if (!ownerChar)
			return;
		
		SCR_CharacterControllerComponent ownerController = SCR_CharacterControllerComponent.Cast(ownerChar.GetCharacterController());
		if (!ownerController)
			return;
		
		ownerController.KRP_ToggleRagdollPose(false);
	}
}
