//------------------------------------------------------------------------------------------------
modded class SCR_CharacterControllerComponent : CharacterControllerComponent
{
	protected IEntity m_pLTS_DroppedWeapon;
	protected static const float MAX_DROPPED_WEAPON_DISTANCE = 5; // meters
	
	//------------------------------------------------------------------------------------------------
	override void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState, bool isJIP)
	{
		super.OnLifeStateChanged(previousLifeState, newLifeState, isJIP);
		
		if (!Replication.IsServer() || newLifeState == ECharacterLifeState.ALIVE)
			return;
		
		BaseWeaponComponent weapon = GetWeaponManagerComponent().GetCurrentWeapon();
		if (weapon)
			m_pLTS_DroppedWeapon = weapon.GetOwner();
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity LTS_GetDroppedWeapon()
	{
		if (!m_pLTS_DroppedWeapon)
			return null;
		
		if (m_pLTS_DroppedWeapon != m_pLTS_DroppedWeapon.GetRootParent())
			return null;
		
		if (vector.Distance(m_pLTS_DroppedWeapon.GetOrigin(), GetOwner().GetOrigin()) > MAX_DROPPED_WEAPON_DISTANCE)
			return null;
		
		return m_pLTS_DroppedWeapon;
	}
}
