//------------------------------------------------------------------------------------------------
class KSC_AIHelper
{
	//------------------------------------------------------------------------------------------------
	//! Change the stance of an AI unit
	static void SetStanceChange(IEntity unit, ECharacterStanceChange stanceChange)
	{
		ECharacterStance stance = -1;
		
		if (stanceChange > 0)
			stance = stanceChange - 1;
		
		AIControlComponent aiCtrlComponent = AIControlComponent.Cast(unit.FindComponent(AIControlComponent));
		if (!aiCtrlComponent)
			return;
		
		AIAgent agent = aiCtrlComponent.GetControlAIAgent();
		if (!agent)
			return;
		
		SCR_AIInfoComponent infoComponent = SCR_AIInfoComponent.Cast(agent.FindComponent(SCR_AIInfoComponent));
		if (!infoComponent)
			return;
			
		CharacterControllerComponent charCtrlComponent = CharacterControllerComponent.Cast(unit.FindComponent(CharacterControllerComponent));
		if (!charCtrlComponent)
			return;
		
		if (stance >= 0)
			infoComponent.SetStance(stance);
		
		charCtrlComponent.SetStanceChange(stanceChange);
	}
}
