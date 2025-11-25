//------------------------------------------------------------------------------------------------
modded class EquipedWeaponStorageComponent : BaseEquipedWeaponStorageComponent
{
	protected ref map<EWeaponType, int> m_mFPM_WeaponTypeCounters = new map<EWeaponType, int>();

	//------------------------------------------------------------------------------------------------
	override protected void OnAddedToSlot(IEntity item, int slotID)
	{
		super.OnAddedToSlot(item, slotID);
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!Replication.IsServer() || !ownerChar)
			return;
		
		BaseWeaponComponent weaponComponent = BaseWeaponComponent.Cast(item.FindComponent(BaseWeaponComponent));
		if (!weaponComponent)
			return;
		
		EWeaponType type = weaponComponent.GetWeaponType();
		m_mFPM_WeaponTypeCounters[type] = m_mFPM_WeaponTypeCounters[type] + 1;
		ownerChar.FPM_UpdateRoleSymbolIcons();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnRemovedFromSlot(IEntity item, int slotID)
	{
		super.OnRemovedFromSlot(item, slotID);
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!Replication.IsServer() || !ownerChar)
			return;
		
		BaseWeaponComponent weaponComponent = BaseWeaponComponent.Cast(item.FindComponent(BaseWeaponComponent));
		if (!weaponComponent)
			return;
		
		EWeaponType type = weaponComponent.GetWeaponType();
		m_mFPM_WeaponTypeCounters[type] = m_mFPM_WeaponTypeCounters[type] - 1;
		ownerChar.FPM_UpdateRoleSymbolIcons();
	}
	
	//------------------------------------------------------------------------------------------------
	int FPM_GetWeaponTypeCount(EWeaponType type)
	{
		return m_mFPM_WeaponTypeCounters[type];
	}
}
