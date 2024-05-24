//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
modded class SCR_RemoveAIGroupCommand : SCR_BaseGroupCommand
{
	//------------------------------------------------------------------------------------------------
	//! Deactivated stay alive on removed AI
	override bool Execute(IEntity cursorTarget, IEntity target, vector targetPosition, int playerID, bool isClient)
	{
		if (!super.Execute(cursorTarget, target, targetPosition, playerID, isClient))
			return false;
		
		if (isClient)
			return true;
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(cursorTarget);
		if (!char)
			return false;
		
		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(char.FindComponent(SCR_CharacterDamageManagerComponent));
		if (damageManager)
			damageManager.SA_Deactivate(char);
		
		return true;
	}
}