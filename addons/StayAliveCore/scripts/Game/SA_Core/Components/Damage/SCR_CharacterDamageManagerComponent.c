//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_DamageManagerComponent
{
	protected bool m_bSA_Initialized = false;
	protected SCR_CharacterHealthHitZone m_pSA_HealthHitZone;
	protected float m_fSA_CriticalHealth;

	//-----------------------------------------------------------------------------------------------------------
	//! Initialize variables
	void SA_Initialize(IEntity owner)
	{
		if (m_bSA_Initialized)
			return;
		
		m_pSA_HealthHitZone = SCR_CharacterHealthHitZone.Cast(GetHitZoneByName("Health"));
		m_fSA_CriticalHealth = m_pSA_HealthHitZone.GetDamageStateThreshold(ECharacterHealthState.CRITICAL);
		m_bSA_Initialized = true;
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Check whether character health state meets requirements for consciousness
	override protected bool ShouldBeUnconscious()
	{
		if (m_bSA_Initialized && m_pSA_HealthHitZone.GetHealthScaled() <= m_fSA_CriticalHealth)
			return true;
		
		return super.ShouldBeUnconscious();
	}
}