//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_DamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	//! Add handlers for protecting players from being killed
	override void SA_Initialize(IEntity owner)
	{
		if (m_bSA_Initialized)
			return;
		
		super.SA_Initialize(owner);
		m_pBloodHitZone.SA_EnableProtection(true);
		m_pSA_HealthHitZone.SA_EnableProtection(true);
	}
}