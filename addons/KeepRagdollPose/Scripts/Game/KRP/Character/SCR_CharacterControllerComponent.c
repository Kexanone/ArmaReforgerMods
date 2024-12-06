//------------------------------------------------------------------------------------------------
//! Remove second chance handling of basic medical
modded class SCR_CharacterControllerComponent : CharacterControllerComponent
{
	[RplProp(onRplName: "KRP_OnUnconsciousPoseChanged")]
	protected KRP_EUnconsciousPose m_eUnconsciousPose;
	
	//------------------------------------------------------------------------------------------------
	protected void KRP_OnUnconsciousPoseChanged()
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
		
		animComponent.SetVariableInt(animComponent.BindVariableInt("UnconsciousPose"), m_eUnconsciousPose);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Register unconscious pose changes in the animation graph
	protected override void OnAnimationEvent(AnimationEventID animEventType, AnimationEventID animUserString, int intParam, float timeFromStart, float timeToEnd)
	{
		super.OnAnimationEvent(animEventType, animUserString, intParam, timeFromStart, timeToEnd);
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return;
		
		RplComponent rpl = char.GetRplComponent();
		if (!rpl || rpl.Role() != RplRole.Authority)
			return;
		
		CharacterAnimationComponent animComponent = char.GetAnimationComponent();
		if (!animComponent)
			return;
		
		if (animEventType != animComponent.BindEvent("KRP_Event_UnconsciousPoseChange"))
			return;
		
		if (intParam == m_eUnconsciousPose)
			return;
		
		KRP_Reposition(intParam);
	}
	
	//------------------------------------------------------------------------------------------------
	void KRP_Reposition(KRP_EUnconsciousPose pose)
	{
		m_eUnconsciousPose = pose;
		Replication.BumpMe();
		KRP_OnUnconsciousPoseChanged();
	}
	
	//------------------------------------------------------------------------------------------------
	KRP_EUnconsciousPose KRP_GetUnconsciousPose()
	{
		return m_eUnconsciousPose;
	}
}

//------------------------------------------------------------------------------------------------
enum KRP_EUnconsciousPose
{
	NONE = 0,
	BACK = 4,
	RECOVERY = 5
}
