//------------------------------------------------------------------------------------------------
modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	protected EMilitarySymbolIcon m_eFPM_RoleSymbolIcons = EMilitarySymbolIcon.INFANTRY;
	protected ref ScriptInvoker m_FPM_OnRoleSymbolIconsChangedInvoker;
	
	//------------------------------------------------------------------------------------------------
	void FPM_UpdateRoleSymbolIcons()
	{
		m_eFPM_RoleSymbolIcons = FPM_ComputeRoleSymbolIcons();
		
		if (m_FPM_OnRoleSymbolIconsChangedInvoker)
			m_FPM_OnRoleSymbolIconsChangedInvoker.Invoke(this, m_eFPM_RoleSymbolIcons);
	}
	
	//------------------------------------------------------------------------------------------------
	protected EMilitarySymbolIcon FPM_ComputeRoleSymbolIcons()
	{
		SCR_EditableCharacterComponent edit = SCR_EditableCharacterComponent.Cast(FindComponent(SCR_EditableCharacterComponent));
		if (!edit)
			return FPM_GetBaseRoleSymbolIcons();
		
		SCR_EditableEntityUIInfo uiInfo = SCR_EditableEntityUIInfo.Cast(edit.GetInfo());
		if (!uiInfo)
			return FPM_GetBaseRoleSymbolIcons();
		
		if (uiInfo.HasEntityLabel(EEditableEntityLabel.ROLE_MEDIC))
			return EMilitarySymbolIcon.MEDICAL;
		
		if (uiInfo.HasEntityLabel(EEditableEntityLabel.ROLE_SAPPER))
			return EMilitarySymbolIcon.MAINTENANCE;
		
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(FindComponent(SCR_InventoryStorageManagerComponent));
		if (!inventoryManager)
			return FPM_GetBaseRoleSymbolIcons();
		
		if (inventoryManager.FPM_GetMedicalKitCount() > 0)
			return EMilitarySymbolIcon.MEDICAL;
		
		if (inventoryManager.FPM_GetWrenchCount() > 0)
			return EMilitarySymbolIcon.MAINTENANCE;
		
		EquipedWeaponStorageComponent equipedWeaponStorage = EquipedWeaponStorageComponent.Cast(FindComponent(EquipedWeaponStorageComponent));
		if (!equipedWeaponStorage)
			return FPM_GetBaseRoleSymbolIcons();
		
		if (equipedWeaponStorage.FPM_GetWeaponTypeCount(EWeaponType.WT_ROCKETLAUNCHER) > 0)
			return EMilitarySymbolIcon.ANTITANK;
		
		if (equipedWeaponStorage.FPM_GetWeaponTypeCount(EWeaponType.WT_SNIPERRIFLE) > 0)
			return FPM_GetBaseRoleSymbolIcons() | EMilitarySymbolIcon.SNIPER;
		
		if (equipedWeaponStorage.FPM_GetWeaponTypeCount(EWeaponType.WT_MACHINEGUN) > 0)
			return FPM_GetBaseRoleSymbolIcons() | EMilitarySymbolIcon.MACHINEGUN;
		
		return FPM_GetBaseRoleSymbolIcons();
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker FPM_GetOnRoleSymbolIconsChanged()
	{
		if (!m_FPM_OnRoleSymbolIconsChangedInvoker)
			m_FPM_OnRoleSymbolIconsChangedInvoker = new ScriptInvoker();
		
		return m_FPM_OnRoleSymbolIconsChangedInvoker;
	}
	
	//------------------------------------------------------------------------------------------------
	EMilitarySymbolIcon FPM_GetRoleSymbolIcons()
	{
		return m_eFPM_RoleSymbolIcons;
	}
	
	//------------------------------------------------------------------------------------------------
	EMilitarySymbolIcon FPM_GetBaseRoleSymbolIcons()
	{
		SCR_EditableCharacterComponent edit = SCR_EditableCharacterComponent.Cast(FindComponent(SCR_EditableCharacterComponent));
		if (!edit)
			return EMilitarySymbolIcon.INFANTRY;
		
		SCR_EditableEntityUIInfo uiInfo = SCR_EditableEntityUIInfo.Cast(edit.GetInfo());
		if (uiInfo && uiInfo.HasEntityLabel(EEditableEntityLabel.ROLE_SCOUT))
			return EMilitarySymbolIcon.RECON;
		
		return EMilitarySymbolIcon.INFANTRY;
	}
}
