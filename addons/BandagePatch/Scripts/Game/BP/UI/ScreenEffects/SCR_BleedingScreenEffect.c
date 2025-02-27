//------------------------------------------------------------------------------------------------
modded class SCR_BleedingScreenEffect : SCR_BaseScreenEffect
{
	//------------------------------------------------------------------------------------------------
	override protected void OnDamageEffectRemoved(notnull SCR_DamageEffect dmgEffect)
	{
		super.OnDamageEffectRemoved(dmgEffect);
		ClearEffects();
	}
}
