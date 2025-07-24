//------------------------------------------------------------------------------------------------
class KSC_ParadropHelper
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
	static void EjectPassengers(Vehicle vehicle, array<ECompartmentType> compartmentTypes = KSC_ParadropHelper.PASSENGER_EJECT_DEFAULT_COMPARTMENT_TYPES)
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
		ACE_AnimationHelperCompartment helper = ACE_AnimationTools.AnimateWithHelperCompartment(ACE_EAnimationHelperID.KSC_PARACHUTE, passenger);
	}
}
