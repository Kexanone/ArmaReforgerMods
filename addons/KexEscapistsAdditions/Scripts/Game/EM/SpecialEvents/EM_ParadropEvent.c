//------------------------------------------------------------------------------------------------
class EM_ParadropEvent : EM_FlyByEvent
{
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		super.Run();
		ESCT_FactionManager factionManager = ESCT_FactionManager.Cast(GetGame().GetFactionManager());
		FactionKey factionKey = factionManager.GetEnemyFactionKey();
				
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = m_vTargetPos;
		
		foreach (Vehicle aircraft : m_aAircrafts)
		{
			SCR_AIGroup group;
			
			if (factionKey == "USSR")
			{
				group = SCR_AIGroup.Cast(GME_ParadropHelper.SpawnGroupInVehicle(aircraft, "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et"));
			}
			else if (factionKey == "US")
			{
				group = SCR_AIGroup.Cast(GME_ParadropHelper.SpawnGroupInVehicle(aircraft, "{DDF3799FA1387848}Prefabs/Groups/BLUFOR/Group_US_RifleSquad.et"));
			}
			else if (factionKey == "RHS_AFRF")
			{
				group = SCR_AIGroup.Cast(GME_ParadropHelper.SpawnGroupInVehicle(aircraft, "{D5BA898DF743F90B}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_3.0/Group_RHS_RF_MSV_VKPO_3.0_RifleSquad.et"));
			}
			else if (factionKey == "RHS_USAF")
			{
				group = SCR_AIGroup.Cast(GME_ParadropHelper.SpawnGroupInVehicle(aircraft, "{F831DFB4A9B46152}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_RifleSquad.et"));
			}
			else
			{
				PrintFormat("EM_ParadropEvent.Run: Faction not supported: %1", factionKey, level: LogLevel.WARNING);
				return;
			}
			
			GetGame().GetCallqueue().CallLater(EjectPassengers,  m_fStartDistance / m_fSpeed * 1000 - 2.5 * GME_ParadropHelper.PASSENGER_EJECT_DELAY_MS + Math.RandomIntInclusive(0, 1000), false, aircraft);
			AIWaypoint wp = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{EE9A99488B40628B}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_SearchAndDestroy.et"), null, params));
			wp.SetCompletionRadius(m_fMaxFlybyDistance);
			group.AddWaypoint(wp);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void EjectPassengers(Vehicle aircraft)
	{
		if (aircraft)
			GME_ParadropHelper.EjectPassengers(aircraft);
	}
}

//------------------------------------------------------------------------------------------------
class GME_ParadropHelper
{
	static const int PASSENGER_EJECT_DELAY_MS = 2500;
	static const ref array<ECompartmentType> PASSENGER_EJECT_DEFAULT_COMPARTMENT_TYPES = {ECompartmentType.CARGO};
	static const ref array<ECompartmentType> ALL_COMPARTMENT_TYPES = {ECompartmentType.PILOT, ECompartmentType.TURRET, ECompartmentType.CARGO};
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup SpawnGroupInVehicle(Vehicle vehicle, ResourceName groupPrefabName)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		vehicle.GetWorldTransform(params.Transform);
		SCR_AIGroup group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(groupPrefabName), null, params));
		group.SetPermanentLOD(0);
		group.GetGroupUtilityComponent().AddUsableVehicle(SCR_AIVehicleUsageComponent.Cast(vehicle.FindComponent(SCR_AIVehicleUsageComponent)));
		group.GetOnAgentAdded().Insert(OnAgentAddedToSpawnedGroup);
		return group;
	}
	
	//------------------------------------------------------------------------------------------------
	protected static void OnAgentAddedToSpawnedGroup(AIAgent agent)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(agent.GetParentGroup());
		if (!group)
			return;
		
		BaseContainer container = group.GetPrefabData().GetPrefab();
		array<ResourceName> units;
		container.Get("m_aUnitPrefabSlots", units);
		
		if (group.GetAgentsCount() < units.Count())
			return;
		
		group.GetOnAgentAdded().Remove(OnAgentAddedToSpawnedGroup);
		
		array<IEntity> vehicles = {};
		group.GetGroupUtilityComponent().m_VehicleMgr.GetAllVehicleEntities(vehicles);
		if (vehicles.IsEmpty())
			return;
		
		Vehicle vehicle = Vehicle.Cast(vehicles[0]);
		if (!vehicle)
			return;
		
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		array<BaseCompartmentSlot> compartments = {};
		compartmentManager.GetFreeCompartmentsOfType(compartments, ECompartmentType.CARGO);
		
		array<AIAgent> agents = {};
		group.GetAgents(agents);
		
		foreach (int i, AIAgent otherAgent : agents)
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(otherAgent.GetControlledEntity());
			
			if (i >= compartments.Count())
			{
				SCR_EntityHelper.DeleteEntityAndChildren(char);
				continue;
			}
			
			SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(char.GetCompartmentAccessComponent());
			compartmentAccess.MoveInVehicle(vehicle, ECompartmentType.CARGO, false, compartments[i]);
			otherAgent.SetPermanentLOD(0);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	static void EjectPassengers(Vehicle vehicle, array<ECompartmentType> compartmentTypes = GME_ParadropHelper.PASSENGER_EJECT_DEFAULT_COMPARTMENT_TYPES)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;

		array<SCR_ChimeraCharacter> toEject = {};
		array<BaseCompartmentSlot> compartmentSlots = {};
		compartmentManager.GetCompartments(compartmentSlots);

		foreach (BaseCompartmentSlot slot : compartmentSlots)
		{
			if (!compartmentTypes.Contains(slot.GetType()))
				continue;

			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(slot.GetOccupant());
			if (!char)
				continue;

			toEject.Insert(char);
		}

		EjectPassengers(toEject);
	}

	//------------------------------------------------------------------------------------------------
	static void EjectPassengers(array<SCR_ChimeraCharacter> characters)
	{
		GetGame().GetCallqueue().CallLater(EjectStep, PASSENGER_EJECT_DELAY_MS, false, characters);
	}
	
	//------------------------------------------------------------------------------------------------
	static void EjectStep(array<SCR_ChimeraCharacter> passengers)
	{
		SCR_ChimeraCharacter passenger = passengers[0];
		passengers.Remove(0);
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(passenger.GetCompartmentAccessComponent());

		Vehicle aircraft = Vehicle.Cast(compartmentAccess.GetCompartment().GetVehicle());
		if (!aircraft)
			return;
		
		int doorIdx = compartmentAccess.GetCompartment().PickDoorIndexForPoint(passenger.GetOrigin());
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(aircraft.FindComponent(SCR_BaseCompartmentManagerComponent));
		vector transform[4];
		compartmentManager.GetDoorInfo(doorIdx).GetEntryPointInfo().GetWorldTransform(transform);
		transform[3] = transform[3] - 2.5 * transform[2];
		compartmentAccess.GetOutVehicle_NoDoor(transform, false, false);
		compartmentAccess.GetOnCompartmentLeft().Insert(OnPassengerEjected);
		
		if (!passengers.IsEmpty())
		{
			GetGame().GetCallqueue().CallLater(EjectStep, PASSENGER_EJECT_DELAY_MS, false, passengers);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	static void OnPassengerEjected(IEntity targetEntity, BaseCompartmentManagerComponent manager, int mgrID, int slotID, bool move)
	{
		BaseCompartmentSlot compartment = manager.FindCompartment(slotID, mgrID);
		if (!compartment)
			return;

		SCR_ChimeraCharacter passenger = SCR_ChimeraCharacter.Cast(compartment.GetOccupant());
		if (!passenger)
			return;
		
		passenger.GetPhysics().SetActive(ActiveState.ACTIVE);
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(passenger.FindComponent(SCR_CompartmentAccessComponent));
		compartmentAccess.GetOnCompartmentLeft().Remove(OnPassengerEjected);
		passenger.GetDamageManager().EnableDamageHandling(false);
		GetGame().GetCallqueue().CallLater(AddChute, 2000, false, passenger);
	}
	
	//------------------------------------------------------------------------------------------------
	static void AddChute(SCR_ChimeraCharacter passenger)
	{
		passenger.GetPhysics().SetActive(ActiveState.ACTIVE);
		passenger.GetDamageManager().EnableDamageHandling(true);
		
		SCR_CompartmentAccessComponent compartmentAccessComponent = SCR_CompartmentAccessComponent.Cast(passenger.GetCompartmentAccessComponent());
		if (!compartmentAccessComponent)
			return;
				
		Resource res = Resource.Load("{6B2915901ADE376F}Prefabs/Vehicles/Core/TESTCARGO.et");
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		passenger.GetWorldTransform(params.Transform);
		Bacon_6198EC294DEC63C0_DeployedParachuteEntity chute = Bacon_6198EC294DEC63C0_DeployedParachuteEntity.Cast(GetGame().SpawnEntityPrefab(res, null, params));
		compartmentAccessComponent.MoveInVehicle(chute, ECompartmentType.CARGO);
		chute.GetPhysics().SetVelocity(passenger.GetPhysics().GetVelocity());
		chute.EM_SetIsAIControlled(true);
		chute.SetEventMask(EntityEvent.FRAME | EntityEvent.SIMULATE);
	}
}

//------------------------------------------------------------------------------------------------
//! Add modifications for supporting AI
modded class Bacon_6198EC294DEC63C0_DeployedParachuteEntity: GenericEntity
{
	[RplProp()]
	protected bool m_bEM_IsAIControlled = false;
	[RplProp()]
	protected bool m_bEM_IsTerminating = false;
	
	protected float m_fEM_MorphProgress = 0;
	
	//------------------------------------------------------------------------------------------------
	void EM_SetIsAIControlled(bool isAIControlled)
	{
		m_bEM_IsAIControlled = isAIControlled;
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Eject AI when touching surface
	override void EOnContact(IEntity owner, IEntity other, Contact contact)
	{
		super.EOnContact(owner, other, contact);
		
		if (!m_bEM_IsAIControlled || !Replication.IsServer())
			return;
		
		BaseCompartmentManagerComponent compartmentManager = BaseCompartmentManagerComponent.Cast(FindComponent(BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;
		
		array<BaseCompartmentSlot> compartments = {};
		compartmentManager.GetCompartments(compartments);
		compartments[0].EjectOccupant();
		m_bEM_IsTerminating = true;
		Replication.BumpMe();
		ClearEventMask(EntityEvent.CONTACT);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Increase falling speed
	override void EOnSimulate(IEntity owner, float timeSlice)
	{
		super.EOnSimulate(owner, timeSlice);
		
		if (!m_bEM_IsAIControlled || !m_RplComponent.IsOwner())
			return;
		
		m_Physics.ApplyImpulse(vector.Up * m_fDownwardForce * timeSlice);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add morph transition for closing chute
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);	
		
		if (!m_bEM_IsAIControlled || !m_bEM_IsTerminating)
			return;
		
		m_fEM_MorphProgress += timeSlice / 2;
		
		if (m_fEM_MorphProgress < 1)
			m_Animation.SetMorphState(m_MorphTarget_Open, m_fEM_MorphProgress);
		else		
			SCR_EntityHelper.DeleteEntityAndChildren(this);
	}
}
