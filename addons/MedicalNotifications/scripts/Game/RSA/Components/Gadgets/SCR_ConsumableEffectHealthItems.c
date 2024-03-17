//------------------------------------------------------------------------------------------------	
modded class SCR_ConsumableEffectHealthItems : SCR_ConsumableEffectBase
{
	//------------------------------------------------------------------------------------------------
	override void ApplyEffect(notnull IEntity target, notnull IEntity user, IEntity item, ItemUseParameters animParams)
	{
		super.ApplyEffect(target, user, item, animParams);
		
		if (!item)
			return;
				
		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(target.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
		SCR_ChimeraCharacter medic = SCR_ChimeraCharacter.Cast(item.GetParent());
		charCtrl.RSA_MedicalFeed_SetLastMedic(medic);
	}
}
