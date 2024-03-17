//-----------------------------------------------------------------------------------------------------------
//! Modifying hit zone for adding health protection handling
modded class SCR_HitZone : HitZone
{
	protected bool m_bSA_ProtectionEnabled = false;

	//-----------------------------------------------------------------------------------------------------------
	//! Enable/disable protection handling
	void SA_EnableProtection(bool enabled)
	{
		m_bSA_ProtectionEnabled = enabled;
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Add health protection handling
	override float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
	{
		if (m_bSA_ProtectionEnabled)
			return SA_ComputeProtectedEffectiveDamage(damageContext, isDOT);
		
		return super.ComputeEffectiveDamage(damageContext, isDOT);
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Computes the effective damage and ensures that the hit zone health can never drop below 1
	protected float SA_ComputeProtectedEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
	{
		float effectiveDamage = super.ComputeEffectiveDamage(damageContext, isDOT);
		
		float remainingHealth = GetHealth() - effectiveDamage;
		if (remainingHealth < 1)
			return GetHealth() - 1;
		
		return effectiveDamage;
	}
}
