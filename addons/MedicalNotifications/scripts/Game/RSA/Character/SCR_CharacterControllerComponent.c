//------------------------------------------------------------------------------------------------
modded class SCR_CharacterControllerComponent : CharacterControllerComponent
{
	protected bool m_bRSA_MedicalFeed_Initialized = false;
	protected int m_iRSA_PlayerId;
	protected SCR_CharacterDamageManagerComponent m_pRSA_DamageManagers;
	protected SCR_NotificationSenderComponent m_pRSA_NotificationSender;
	protected ECharacterLifeState m_iRSA_PreviousLifeState = ECharacterLifeState.ALIVE;
	protected IEntity m_pRSA_LastMedic;
	
	//------------------------------------------------------------------------------------------------
	void RSA_MedicalFeed_Initialize(int playerId)
	{
		if (m_bRSA_MedicalFeed_Initialized)
			return;
		
		m_iRSA_PlayerId = playerId;
		m_pRSA_DamageManagers = SCR_CharacterDamageManagerComponent.Cast(GetOwner().FindComponent(SCR_CharacterDamageManagerComponent));
		m_pRSA_NotificationSender = SCR_NotificationSenderComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_NotificationSenderComponent));
		m_OnLifeStateChanged.Insert(RSA_MedicalFeed_OnLifeStateChanged);
		m_bRSA_MedicalFeed_Initialized = true;
	}
	
	//------------------------------------------------------------------------------------------------
	void RSA_MedicalFeed_SetLastMedic(IEntity medic)
	{
		m_pRSA_LastMedic = medic;
	}
	
	//------------------------------------------------------------------------------------------------
	void RSA_MedicalFeed_OnLifeStateChanged(ECharacterLifeState lifeState)
	{
		if (lifeState == m_iRSA_PreviousLifeState)
			return;
		
		switch (lifeState)
		{
			// Add protection handler when revived
			case ECharacterLifeState.ALIVE:
			{
				Print(GetOwner() == m_pRSA_LastMedic);
				m_pRSA_NotificationSender.OnControllableRevived(GetOwner(), m_pRSA_LastMedic);
				break;
			};
			
			// Schedule removal of protection handler when falling unconscious
			case ECharacterLifeState.INCAPACITATED:
			{
				m_pRSA_NotificationSender.OnControllableIncapacitated(GetOwner(), m_pRSA_DamageManagers.GetInstigator().GetInstigatorEntity());
				m_pRSA_LastMedic = null;
				break;
			};
		};
		
		m_iRSA_PreviousLifeState = lifeState;
	}
}