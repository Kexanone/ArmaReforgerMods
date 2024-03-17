//-----------------------------------------------------------------------------------------------------------
//! Modifying health hit zone for triggering unconscious checks
modded class SCR_CharacterHealthHitZone : SCR_HitZone
{
	//-----------------------------------------------------------------------------------------------------------
	//! Trigger unconscious check when health state has changed
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamageStateChanged()
	{
		super.OnDamageStateChanged();

		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
		if (damageManager)
			damageManager.UpdateConsciousness();
	}
}