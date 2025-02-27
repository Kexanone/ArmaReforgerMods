//-----------------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerEvaluator : DamageEffectEvaluator
{
	protected static const float BP_CRITICAL_DAMAGE_THRESHOLD_SCALE = 0.5206;

	//-----------------------------------------------------------------------------------------------------------
	override private void EffectCauseBleeding(SCR_InstantDamageEffect effect, ExtendedDamageManagerComponent dmgManager)
	{
		SCR_CharacterHitZone affectedHitZone = SCR_CharacterHitZone.Cast(effect.GetAffectedHitZone());
		if (!affectedHitZone)
			return;
		
		vector hitPosDirNorm[3];
		SCR_DamageContext context = new SCR_DamageContext(
			effect.GetDamageType(), 
			effect.GetTotalDamage(), 
			hitPosDirNorm,
			dmgManager.GetOwner(), 
			affectedHitZone, 
			effect.GetInstigator(),
			null,
			-1,
			-1);
		
		float var = affectedHitZone.ComputeEffectiveDamage(context, false);
		if (var < affectedHitZone.GetCriticalDamageThreshold() * affectedHitZone.GetMaxHealth() * BP_CRITICAL_DAMAGE_THRESHOLD_SCALE)
			return; 
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
		if (!damageMgr)
			return;
		
		damageMgr.AddBleedingEffectOnHitZone(affectedHitZone);
	}
}
