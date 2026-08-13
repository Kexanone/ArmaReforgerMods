//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
modded class SCR_RecruitAIGroupCommand : SCR_BaseGroupCommand
{
	//------------------------------------------------------------------------------------------------
	//! Activate stay alive on recruited AI
	override bool Execute(IEntity cursorTarget, IEntity groupEnt, vector targetPosition, int playerID, bool isClient)
	{
		if (!super.Execute(cursorTarget, groupEnt, targetPosition, playerID, isClient))
			return false;
		
		if (isClient)
			return true;
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(cursorTarget);
		if (!char)
			return false;
		
		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(char.FindComponent(SCR_CharacterDamageManagerComponent));
		if (damageManager)
			damageManager.SA_Activate(char);
		
		return true;
	}
}