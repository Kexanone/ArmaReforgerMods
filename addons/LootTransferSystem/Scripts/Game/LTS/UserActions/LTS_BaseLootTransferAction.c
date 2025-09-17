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
		JWK_PlayerWantedManagerComponent wantedManager = JWK.GetPlayerWanted();
		if (wantedManager)
			wantedManager.NotifyPlayerLootAction_S(pUserEntity, pOwnerEntity);
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
		
		// Unload items in items
		BaseInventoryStorageComponent innerStorage = BaseInventoryStorageComponent.Cast(item.FindComponent(BaseInventoryStorageComponent));
		if (innerStorage)
		{
			array<IEntity> innerItems = {};
			innerStorage.GetAll(innerItems);
			
			foreach (IEntity innerItem : innerItems)
			{
				TransferItem(innerItem, targetStorageManager, targetStorage);
			}
		}
		
		// Do not remove attachments and pouches from vests
		if (IsAttachedWeaponAttachment(item) || IsAttachedHelmetAttachment(item) || IsAttachedArmorPlate(item) || IsPouch(item))
			return true;
		
		return (targetStorageManager.TryMoveItemToStorage(item, targetStorage) || targetStorageManager.TryInsertItem(item));
	}
	
	//------------------------------------------------------------------------------------------------
	protected IEntity GetParentSlotOwner(IEntity item)
	{
		InventoryItemComponent itemComponent = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
		if (!itemComponent)
			return null;
		
		InventoryStorageSlot slot = itemComponent.GetParentSlot();
		if (!slot)
			return null;
		
		BaseInventoryStorageComponent storage = slot.GetStorage();
		if (!storage)
			return null;
		
		return storage.GetOwner();
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsAttachedWeaponAttachment(IEntity item)
	{
		IEntity parent = item.GetParent();
		if (parent && !ChimeraCharacter.Cast(parent) && parent.FindComponent(BaseWeaponComponent))
			return true;
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsAttachedArmorPlate(IEntity item)
	{
		IEntity parentSlotOwner = GetParentSlotOwner(item);
		if (!parentSlotOwner)
			return false;
		
		BaseLoadoutClothComponent clothComponent = BaseLoadoutClothComponent.Cast(parentSlotOwner.FindComponent(BaseLoadoutClothComponent));
		if (!clothComponent)
			return false;
		
		return LoadoutArmoredVestSlotArea.Cast(clothComponent.GetAreaType());
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsAttachedHelmetAttachment(IEntity item)
	{
		IEntity parentSlotOwner = GetParentSlotOwner(item);
		if (!parentSlotOwner)
			return false;
		
		BaseLoadoutClothComponent clothComponent = BaseLoadoutClothComponent.Cast(parentSlotOwner.FindComponent(BaseLoadoutClothComponent));
		if (!clothComponent)
			return false;
		
		return LoadoutHeadCoverArea.Cast(clothComponent.GetAreaType());
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsPouch(IEntity item)
	{
		// BaseInventoryStorageComponent indicates that it has an inner storage
		BaseInventoryStorageComponent itemComponent = BaseInventoryStorageComponent.Cast(item.FindComponent(BaseInventoryStorageComponent));
		if (!itemComponent)
			return false;
		
		InventoryStorageSlot slot = itemComponent.GetParentSlot();
		if (!slot)
			return false;
		
		return ClothNodeStorageComponent.Cast(slot.GetStorage());
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
