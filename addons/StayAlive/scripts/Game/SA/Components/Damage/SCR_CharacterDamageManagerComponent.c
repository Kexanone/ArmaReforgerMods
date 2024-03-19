//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_DamageManagerComponent
{
	protected const float SA_UNCONSCIOUS_PROTECTION_TIMEOUT = 1000;

	//-----------------------------------------------------------------------------------------------------------
	//! Add handlers for protecting players that fall unconscious
	override void SA_Initialize(IEntity owner)
	{
		if (m_bSA_Initialized)
			return;
		
		super.SA_Initialize(owner);
		
		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
		charCtrl.m_OnLifeStateChanged.Insert(SA_OnLifeStateChanged);
		m_pSA_HealthHitZone.SA_EnableProtection(true);
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Add/Remove protection handler when consciousness changes
	protected void SA_OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		switch (newLifeState)
		{
			// Add protection handler when revived
			case ECharacterLifeState.ALIVE:
			{
				GetGame().GetCallqueue().Remove(m_pSA_HealthHitZone.SA_EnableProtection);
				m_pSA_HealthHitZone.SA_EnableProtection(true);
				break;
			};
			
			// Schedule removal of protection handler when falling unconscious
			case ECharacterLifeState.INCAPACITATED:
			{
				GetGame().GetCallqueue().CallLater(m_pSA_HealthHitZone.SA_EnableProtection, SA_UNCONSCIOUS_PROTECTION_TIMEOUT, false, false);
				break;
			};
			
			// Remove protection handler when dead
			case ECharacterLifeState.DEAD:
			{
				GetGame().GetCallqueue().Remove(m_pSA_HealthHitZone.SA_EnableProtection);
				m_pSA_HealthHitZone.SA_EnableProtection(false);
				break;
			};
		};
	}
}