/*
//------------------------------------------------------------------------------------------------
modded class SCR_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	override protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item)
	{		
		super.OnItemAdded(storageOwner, item);
		
		if (item.FindComponent(InventoryItemComponent))
			GCP_System.Withdraw(item);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnItemRemoved(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
		super.OnItemRemoved(storageOwner, item);
		
		// Put everything on the queue, except mines
		if (!SCR_PressureTriggerComponent.Cast(item.FindComponent(SCR_PressureTriggerComponent)))
			GCP_System.Insert(item);
	}
}
*/