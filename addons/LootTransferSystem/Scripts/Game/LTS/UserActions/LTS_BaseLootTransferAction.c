//------------------------------------------------------------------------------------------------
class LTS_BaseLootTransferAction : ScriptedUserAction
{
	[Attribute(desc: "String for transfer from/to %1")]
	protected LocalizedString m_sSpecificTransfer;
	
	[Attribute(defvalue: "No vehicles nearby", desc: "String when no vehicles are nearby")]
	protected LocalizedString m_sNoVehiclesNearby;
	
	[Attribute(defvalue: "25", desc: "Maximum distance from vehicle [m]", params: "0 inf")]
	protected float m_fMaxDistance;
	
	protected static const float VEHICLE_QUERY_TIMEOUT = 1;
	protected float m_fVehicleQueryTimer = VEHICLE_QUERY_TIMEOUT;
	protected Vehicle m_pNearestVehicle;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		UpdateNearestVehicle();
		if (!m_pNearestVehicle)
			return;
		
		ScriptedInventoryStorageManagerComponent targetStorageManager = GetTargetStorageManager();
		if (!targetStorageManager)
			return;
		
		BaseInventoryStorageComponent targetStorage = GetTargetStorage();
		if (!targetStorage)
			return;
		
		int failureCounter = 0;
		
		foreach (IEntity item : FindItemsToTransfer())
		{
			if (!TransferItem(item, targetStorageManager, targetStorage))
				++failureCounter;
		}
		
	#ifdef FREEDOM_FIGHTERS
		JWK.GetPlayerWanted().NotifyPlayerLootAction_S(pUserEntity, pOwnerEntity);
	#endif
		
		int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity);
		if (playerID == 0)
			return;
		
		if (failureCounter == 0)
			SCR_NotificationsComponent.SendToPlayer(playerID, ENotification.LTS_TRANSFER_COMPLETED);
		else
			SCR_NotificationsComponent.SendToPlayer(playerID, ENotification.LTS_TRANSFER_INCOMPLETE, failureCounter);
	}
	
	//------------------------------------------------------------------------------------------------
	protected ScriptedInventoryStorageManagerComponent GetTargetStorageManager();	
	
	//------------------------------------------------------------------------------------------------
	protected BaseInventoryStorageComponent GetTargetStorage();
	
	//------------------------------------------------------------------------------------------------
	protected ScriptedInventoryStorageManagerComponent GetSourceStorageManager();
	
	//------------------------------------------------------------------------------------------------
	protected array<IEntity> FindItemsToTransfer()
	{
		ScriptedInventoryStorageManagerComponent sourceStorageManager = GetSourceStorageManager();
		if (!sourceStorageManager)
			return {};
		
		array<IEntity> items = {};
		sourceStorageManager.GetItems(items);
		return items;
	}

	//------------------------------------------------------------------------------------------------
	protected bool TransferItem(IEntity item, ScriptedInventoryStorageManagerComponent targetStorageManager, BaseInventoryStorageComponent targetStorage)
	{
		// Skip if item has already been transferred
		if (targetStorageManager.Contains(item))
			return true;
		
		// Unload items in items, except weapon attachments
		BaseInventoryStorageComponent pouchStorage = BaseInventoryStorageComponent.Cast(item.FindComponent(BaseInventoryStorageComponent));
		if (pouchStorage && !WeaponAttachmentsStorageComponent.Cast(pouchStorage))
		{
			array<IEntity> pouchItems = {};
			pouchStorage.GetAll(pouchItems);
			
			foreach (IEntity pouchItem : pouchItems)
			{
				// Unload items from subpouches, but do not unload subpouch from pouch
				BaseInventoryStorageComponent subPouchStorage = BaseInventoryStorageComponent.Cast(pouchItem.FindComponent(BaseInventoryStorageComponent));
				if (subPouchStorage)
				{
					array<IEntity> subPouchItems = {};
					subPouchStorage.GetAll(subPouchItems);
					
					foreach (IEntity subPouchItem : subPouchItems)
					{
						TransferItem(subPouchItem, targetStorageManager, targetStorage);
					}
				}
				else
				{
					TransferItem(pouchItem, targetStorageManager, targetStorage);
				}
			}
		}
		
		return (targetStorageManager.TryMoveItemToStorage(item, targetStorage) || targetStorageManager.TryInsertItem(item));
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateNearestVehicle()
	{
		LTF_WorldQueryTool<Vehicle> query = new LTF_WorldQueryTool<Vehicle>();
		m_pNearestVehicle = query.FindNearest(GetOwner().GetOrigin(), m_fMaxDistance);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return !CompartmentAccessComponent.GetVehicleIn(user);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		m_fVehicleQueryTimer += GetGame().GetWorld().GetTimeSlice();
		if (m_fVehicleQueryTimer >= VEHICLE_QUERY_TIMEOUT)
		{
			m_fVehicleQueryTimer = 0;
			UpdateNearestVehicle();
		}
		
		if (!m_pNearestVehicle)
		{
			SetCannotPerformReason(m_sNoVehiclesNearby);
			return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		if (!m_pNearestVehicle)
			return false;
		
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.GetEditableEntity(m_pNearestVehicle);
		if (!editable)
			return false;
		
		SCR_UIInfo info = editable.GetInfo();
		if (!info || info.GetName().IsEmpty())
			return false;
		
		outName = string.Format(m_sSpecificTransfer,  info.GetName());
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript() { return false; };
}
