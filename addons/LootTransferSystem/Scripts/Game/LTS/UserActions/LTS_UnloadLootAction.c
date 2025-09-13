//------------------------------------------------------------------------------------------------
class LTS_UnloadLootAction : LTS_BaseLootTransferAction
{
	//------------------------------------------------------------------------------------------------
	override protected ScriptedInventoryStorageManagerComponent GetTargetStorageManager()
	{
		return ScriptedInventoryStorageManagerComponent.Cast(GetOwner().FindComponent(SCR_InventoryStorageManagerComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected BaseInventoryStorageComponent GetTargetStorage()
	{
		return BaseInventoryStorageComponent.Cast(GetOwner().FindComponent(BaseInventoryStorageComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ScriptedInventoryStorageManagerComponent GetSourceStorageManager()
	{
		return ScriptedInventoryStorageManagerComponent.Cast(m_pNearestVehicle.FindComponent(SCR_VehicleInventoryStorageManagerComponent));
	}
}
