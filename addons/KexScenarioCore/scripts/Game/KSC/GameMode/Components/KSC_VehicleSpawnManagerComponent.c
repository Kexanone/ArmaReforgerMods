//------------------------------------------------------------------------------------------------
class KSC_VehicleSpawnManagerComponentClass : SCR_BaseGameModeComponentClass
{
};

//------------------------------------------------------------------------------------------------
//! Manages KSC_VehicleSpawnSlot entities
class KSC_VehicleSpawnManagerComponent : SCR_BaseGameModeComponent
{
	[Attribute("5", UIWidgets.EditBox, desc: "Timeout in seconds for the respawn handling")]
	protected float m_fHandlerTimeout;
	protected float m_fHandlerTimer = 0;
	
	[Attribute("30", UIWidgets.EditBox, desc: "Number of seconds after which vehicle respawns when no player is with the deserted distance")]
	protected float m_fDesertedDelay;
	
	[Attribute("1000", UIWidgets.EditBox, desc: "Vehicle is considered deserted if all players are farther away than this distance (in meters)")]
	protected float m_fDesertedPlayerDistance;
	protected float m_fDesertedPlayerDistanceSq;
	
	[Attribute("1.5", UIWidgets.EditBox, desc: "Deserted handling is active if vehicle is farther away from the initial spawn position than this distance (in meters)")]
	protected float m_fDesertedSlotDistance;
	protected float m_fDesertedSlotDistanceSq;
	
	protected ref array<KSC_VehicleSpawnSlot> m_aManagedSlots = {};
	
	//------------------------------------------------------------------------------------------------
	//! Enable FRAME event on the server
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!Replication.IsServer())
			return;
		
		Activate(owner);
		SetEventMask(owner, EntityEvent.FRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Check all spawn slots and schedule respawns if needed
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		
		if (!Replication.IsServer())
			return;
		
		m_fHandlerTimer += timeSlice;
		
		if (m_fHandlerTimer < m_fHandlerTimeout)
			return;
		
		m_fHandlerTimer = 0;
		array<IEntity> players = KSC_PlayerHelper.GetPlayers();
		
		foreach (KSC_VehicleSpawnSlot slot : m_aManagedSlots)
		{
			// Skip, since asset is already about to be respawned
			if (slot.GetIsRespawning())
				continue;
			
			IEntity vehicle = slot.GetVehicle();
			
			// Respawn if asset has been deleted, destroyed or it is under water
			if (!vehicle || !slot.GetIsVehicleAlive())
			{
				slot.ScheduleRespawn();
				continue;
			};
			
			// Respawn if engine has been drowned
			VehicleControllerComponent_SA vehicleController = VehicleControllerComponent_SA.Cast(vehicle.FindComponent(VehicleControllerComponent_SA));
			if (vehicleController && vehicleController.GetEngineDrowned())
			{
				slot.ScheduleRespawn();
				continue;
			};
			
			UpdateDesertedState(slot, players);
			
			// Respawn if vehicle has been deserted for too long
			if (slot.GetDesertedDuration() >= m_fDesertedDelay)
				slot.ScheduleRespawn();
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Updates the deserted state of the vehicle associated with the slot
	protected void UpdateDesertedState(KSC_VehicleSpawnSlot slot, array<IEntity> players)
	{
		vector vehiclePos = slot.GetVehicle().GetOrigin();
		
		// Skip deserted check if vehicle is close to spawn
		if (vector.DistanceSqXZ(slot.GetOrigin(), vehiclePos) < m_fDesertedSlotDistanceSq)
		{
			slot.SetDeserted(false);
			return;
		};
				
		foreach (IEntity player : players)
		{
			// Skip deserted check if player is close
			if (vector.DistanceSqXZ(player.GetOrigin(), vehiclePos) < m_fDesertedPlayerDistanceSq)
			{
				slot.SetDeserted(false);
				return;
			};
		};
		
		slot.SetDeserted(true);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Register spawn slot to be handled by the manager
	void RegisterSlot(KSC_VehicleSpawnSlot slot)
	{
		slot.Respawn();
		m_aManagedSlots.Insert(slot);
	}
};