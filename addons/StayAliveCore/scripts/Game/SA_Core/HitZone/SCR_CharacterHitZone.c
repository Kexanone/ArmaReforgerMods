//-----------------------------------------------------------------------------------------------------------
//! Modifying health hit zone for triggering unconscious checks
modded class SCR_CharacterHealthHitZone : SCR_HitZone
{
	//-----------------------------------------------------------------------------------------------------------
	//! Trigger unconscious check when health state has changed
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamageStateChanged(EDamageState newState, EDamageState previousDamageState, bool isJIP)
	{
		super.OnDamageStateChanged(newState, previousDamageState, isJIP);

		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
		if (damageManager)
			damageManager.UpdateConsciousness();
	}
}

//-----------------------------------------------------------------------------------------------------------
//! Modifying physical hit zones for triggering unconscious checks
modded class SCR_CharacterHitZone : SCR_RegeneratingHitZone
{
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamageStateChanged(EDamageState newState, EDamageState previousDamageState, bool isJIP)
	{
		super.OnDamageStateChanged(newState, previousDamageState, isJIP);
		
		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
		if (damageManager)
			damageManager.UpdateConsciousness();
	}
}
