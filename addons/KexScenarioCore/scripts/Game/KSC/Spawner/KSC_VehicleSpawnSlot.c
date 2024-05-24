/*
//------------------------------------------------------------------------------------------------
class KSC_VehicleSpawnSlotClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_VehicleSpawnSlot : GenericEntity
{
	[Attribute(desc: "Vehicle prefab to spawn on this slot")]
	protected ResourceName m_sVehiclePrefabToSpawn;
	
	[Attribute(defvalue: "3", desc: "How long it takes for the vehicle to respawn")]
	protected float m_iRespawnTime;
	
	protected Vehicle m_pVehicle = null;
	protected bool m_bIsRespawning = false;
	protected float m_fDesertedStartTime = -1;
	
#ifdef WORKBENCH
	protected IEntity m_pPreviewEntity;
#endif
	
	//------------------------------------------------------------------------------------------------
	//! Enable INIT event on server
	void KSC_VehicleSpawnSlot(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
				
		Activate();
		SetEventMask(EntityEvent.INIT);
	};
		
	//------------------------------------------------------------------------------------------------
	//! Register the slot at the manager
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!Replication.IsServer())
			return;
		
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (!gameMode)
			return;
		
		KSC_VehicleSpawnManagerComponent spawnManager = KSC_VehicleSpawnManagerComponent.Cast(gameMode.FindComponent(KSC_VehicleSpawnManagerComponent));
		if (!spawnManager)
			return;
		
		spawnManager.RegisterSlot(this);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return the vehicle that got spawned by this slot
	IEntity GetVehicle()
	{
		return m_pVehicle;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return the occupants of the vehicle spawned by this slot
	protected array<CompartmentAccessComponent> GetCrew()
	{
		SCR_EditableVehicleComponent editableVehicle = SCR_EditableVehicleComponent.Cast(m_pVehicle.FindComponent(SCR_EditableVehicleComponent));
		array<CompartmentAccessComponent> crewCompartmentAccess = new array<CompartmentAccessComponent>;
		editableVehicle.GetCrew(crewCompartmentAccess, false);
		return crewCompartmentAccess;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Respawns vehicle after respawn time has passed
	void ScheduleRespawn()
	{
		m_bIsRespawning = true;
		EjectCrew();		
		GetGame().GetCallqueue().CallLater(Respawn, 1000*m_iRespawnTime);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Eject the crew out of the vehicle spawned by this slot
	protected void EjectCrew()
	{
		if (!m_pVehicle)
			return;
		
		SCR_EditableVehicleComponent editableVehicle = SCR_EditableVehicleComponent.Cast(m_pVehicle.FindComponent(SCR_EditableVehicleComponent));
		array<CompartmentAccessComponent> crewCompartmentAccess = GetCrew();

		foreach (CompartmentAccessComponent compartmentAccess: crewCompartmentAccess)
		{
			compartmentAccess.EjectOutOfVehicle();
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Instantly respawn the vehicle unless it is occupied; Kick out passengers and reschedule respawn
	//! otherwise.
	void Respawn()
	{
		if (m_pVehicle)
		{
			// Crew ejection can be severley delayed
			// => If it is not completed, we reschedule the respawn, since otherwise players that are still in the vehicle get deleted too
			if (!GetCrew().IsEmpty())
			{
				EjectCrew();
				GetGame().GetCallqueue().CallLater(Respawn, 1000);
				return;
			};
			
			SCR_EntityHelper.DeleteEntityAndChildren(m_pVehicle);
		};
		
		vector transform[4];
		GetWorldTransform(transform);
		m_pVehicle = KSC_GameTools.SpawnVehiclePrefab(m_sVehiclePrefabToSpawn, transform);
		m_bIsRespawning = false;
	};
	
	//------------------------------------------------------------------------------------------------
	//! Change the deserted state
	//! Sets the deserted start time
	void SetDeserted(bool isDeserted)
	{
		if (isDeserted)
		{
			if (m_fDesertedStartTime < 0)
				m_fDesertedStartTime = GetGame().GetWorld().GetWorldTime();
		}
		else
		{
			m_fDesertedStartTime = -1;
		}	
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns how long the vehicle is deserted in seconds
	float GetDesertedDuration()
	{
		if (m_fDesertedStartTime < 0)
			return 0;
		else
			return (GetGame().GetWorld().GetWorldTime() - m_fDesertedStartTime) / 1000;		
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if the vehicle is currently respawning
	bool GetIsRespawning()
	{
		return m_bIsRespawning;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if the vehicle is alive; false if the vehilce is destroyed or not yet spawned
	bool GetIsVehicleAlive()
	{
		if (!m_pVehicle)
			return false;
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(m_pVehicle.FindComponent(DamageManagerComponent));
		return !damageManager.IsDestroyed();
	}

	
#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------
	void SpawnEntityPreview(Resource resource)
	{
		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(spawnParams.Transform);
			
		m_pPreviewEntity = GetGame().SpawnEntityPrefab(resource, null, spawnParams);
	}
	
	//------------------------------------------------------------------------------------------------
	override void _WB_OnInit(inout vector mat[4], IEntitySource src)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			
		Resource resource = Resource.Load(m_sVehiclePrefabToSpawn);
		if (!resource)
			return;
			
		SpawnEntityPreview(resource);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool _WB_OnKeyChanged(BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
	{
		if (key == "coords")
		{
			SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			
			Resource resource = Resource.Load(m_sVehiclePrefabToSpawn);
			if (!resource)
				return false;
			
			SpawnEntityPreview(resource);
			return true;
		}
		else if (key == "m_sVehiclePrefabToSpawn")
		{
			SCR_EntityHelper.DeleteEntityAndChildren(m_pPreviewEntity);
			return false;
		}
		return false;
	}
#endif
}
*/