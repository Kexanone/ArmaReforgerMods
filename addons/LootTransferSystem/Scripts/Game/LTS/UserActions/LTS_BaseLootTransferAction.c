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
		
		int failureCounter = 0;
		
		foreach (IEntity item : FindItemsToTransfer())
		{
			if (!TransferItem(item, targetStorageManager))
				++failureCounter;
		}
		
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
	protected bool TransferItem(IEntity item, ScriptedInventoryStorageManagerComponent targetStorageManager)
	{
			BaseInventoryStorageComponent targetStorage = targetStorageManager.FindStorageForItem(item, EStoragePurpose.PURPOSE_DEPOSIT);
			if (!targetStorage)
				return false;
			
			return targetStorageManager.TryMoveItemToStorage(item, targetStorage) || targetStorageManager.TryInsertItem(item, EStoragePurpose.PURPOSE_DEPOSIT);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateNearestVehicle()
	{
		LTF_WorldQueryTool<Vehicle> query = new LTF_WorldQueryTool<Vehicle>();
		m_pNearestVehicle = query.FindFirst(GetOwner().GetOrigin(), m_fMaxDistance);
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

//------------------------------------------------------------------------------------------------
class LTF_WorldQueryTool<IEntity T> : Managed
{
	protected ref array<T> m_aResults;
	protected T m_pResult;
	
	//------------------------------------------------------------------------------------------------
	array<T> FindAll(vector center, float radius)
	{
		m_aResults = {};
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, QueryAllCallback);
		return m_aResults;
	}
	
	//------------------------------------------------------------------------------------------------
	T FindFirst(vector center, float radius)
	{
		m_pResult = null;
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, QueryFirstCallback);
		return m_pResult;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryAllCallback(IEntity entity)
	{
		T castedEntity = T.Cast(entity);
		if (castedEntity)
			m_aResults.Insert(castedEntity);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryFirstCallback(IEntity entity)
	{
		T castedEntity = T.Cast(entity);
		if (castedEntity)
		{
			m_pResult = castedEntity;
			return false;
		}
		
		return true;
	}
}
