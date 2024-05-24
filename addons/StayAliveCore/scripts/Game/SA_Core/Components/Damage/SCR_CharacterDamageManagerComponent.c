//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_DamageManagerComponent
{
	protected bool m_bSA_Active = false;
	protected SCR_CharacterHealthHitZone m_pSA_HealthHitZone;
	protected float m_fSA_CriticalHealth;
	protected ref array<HitZone> m_aSA_PhysicalHitZones = {};

	//-----------------------------------------------------------------------------------------------------------
	//! Activate stay alive
	void SA_Activate(IEntity owner)
	{
		if (m_bSA_Active)
			return;
		
		m_pSA_HealthHitZone = SCR_CharacterHealthHitZone.Cast(GetHitZoneByName("Health"));
		m_fSA_CriticalHealth = m_pSA_HealthHitZone.GetDamageStateThreshold(ECharacterHealthState.CRITICAL);
		GetPhysicalHitZones(m_aSA_PhysicalHitZones);
		m_bSA_Active = true;
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Deactivate stay alive
	void SA_Deactivate(IEntity owner)
	{
		if (!m_bSA_Active)
			return;
		
		m_bSA_Active = false;
	}
	
	//-----------------------------------------------------------------------------------------------------------
	bool SA_CanBeHealed()
	{
		foreach (HitZone hitZone : m_aSA_PhysicalHitZones)
		{
			if (hitZone.GetHealthScaled() < 0.999)
				return true;
		}
		
		return false;
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Check whether character health state meets requirements for consciousness
	override protected bool ShouldBeUnconscious()
	{
		if (m_bSA_Active && m_pSA_HealthHitZone.GetHealthScaled() <= m_fSA_CriticalHealth && SA_CanBeHealed())
			return true;
		
		return super.ShouldBeUnconscious();
	}
}