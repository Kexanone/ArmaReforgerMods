modded class SCR_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
	protected EMilitarySymbolIcon m_eFPM_RoleSymbolIcons = EMilitarySymbolIcon.INFANTRY;
	protected int m_iFPM_MedicalKitCount = 0;
	protected int m_iFPM_WrenchCount = 0;
	
	//------------------------------------------------------------------------------------------------
	override protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
		super.OnItemAdded(storageOwner, item);
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!Replication.IsServer() || !ownerChar)
			return;
		
		SCR_BaseSupportStationComponent supportGadgetComponent = SCR_BaseSupportStationComponent.Cast(item.FindComponent(SCR_BaseSupportStationComponent));
		if (!supportGadgetComponent)
			return;
		
		if (SCR_HealSupportStationComponent.Cast(supportGadgetComponent))
		{
			++m_iFPM_MedicalKitCount;
			ownerChar.FPM_UpdateRoleSymbolIcons();
			return;
		}
		
		if (SCR_RepairSupportStationComponent.Cast(supportGadgetComponent))
		{
			++m_iFPM_WrenchCount;
			ownerChar.FPM_UpdateRoleSymbolIcons();
			return;
		}
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnItemRemoved(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
		super.OnItemRemoved(storageOwner, item);
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!Replication.IsServer() || !ownerChar)
			return;
		
		SCR_BaseSupportStationComponent supportGadgetComponent = SCR_BaseSupportStationComponent.Cast(item.FindComponent(SCR_BaseSupportStationComponent));
		if (!supportGadgetComponent)
			return;
		
		if (SCR_HealSupportStationComponent.Cast(supportGadgetComponent))
		{
			--m_iFPM_MedicalKitCount;
			ownerChar.FPM_UpdateRoleSymbolIcons();
			return;
		}
		
		if (SCR_RepairSupportStationComponent.Cast(supportGadgetComponent))
		{
			--m_iFPM_WrenchCount;
			ownerChar.FPM_UpdateRoleSymbolIcons();
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	int FPM_GetMedicalKitCount()
	{
		return m_iFPM_MedicalKitCount;
	}
	
	//------------------------------------------------------------------------------------------------
	int FPM_GetWrenchCount()
	{
		return m_iFPM_WrenchCount;
	}
}
