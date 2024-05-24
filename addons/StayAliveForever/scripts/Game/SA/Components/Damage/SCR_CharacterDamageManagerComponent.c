//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_DamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	//! Add handlers for protecting players from being killed
	override void SA_Activate(IEntity owner)
	{
		if (m_bSA_Active)
			return;
		
		super.SA_Activate(owner);
		m_pBloodHitZone.SA_EnableProtection(true);
		m_pSA_HealthHitZone.SA_EnableProtection(true);
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Remove handlers for protecting players from being killed
	override void SA_Deactivate(IEntity owner)
	{
		if (!m_bSA_Active)
			return;
		
		super.SA_Deactivate(owner);
		m_pBloodHitZone.SA_EnableProtection(false);
		m_pSA_HealthHitZone.SA_EnableProtection(false);
	}
}