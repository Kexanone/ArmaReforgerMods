//------------------------------------------------------------------------------------------------
class KSC_ParadropSystem : GameSystem
{
	[Attribute(defvalue: "50", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "80", desc: "Flying height relative to terrain in meters")]
	protected float m_fHeight;
	
	[Attribute(defvalue: "3000", desc: "Starting distance from player in meters")]
	protected float m_fStartDistance;
	
	[Attribute(defvalue: "6000", desc: "Distance from starting position for triggering despawn in meters")]
	protected float m_fDespawnDistance;
	
	protected ref array<ref KSC_ParadropSystem_HeloContext> m_aHeloContexts = {};
	protected float m_fDistanceCheckTimer = 0;
	protected KSC_FactionManager m_pFactionManager;
	
	protected const float DISTANCE_CHECK_TIMEOUT = 2;
	protected const float TERRAIN_Y_SCAN_STEP_SIZE = 100;
	
	//------------------------------------------------------------------------------------------------
	static KSC_ParadropSystem GetInstance()
	{
		ChimeraWorld world = GetGame().GetWorld();
		return KSC_ParadropSystem.Cast(world.FindSystem(KSC_ParadropSystem));
	}
	
	//------------------------------------------------------------------------------------------------
	override static void InitInfo(WorldSystemInfo outInfo)
	{
		super.InitInfo(outInfo);
		outInfo.SetLocation(ESystemLocation.Server)
			.AddPoint(ESystemPoint.SimulatePhysics);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool ShouldBePaused()
	{
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnInit()
	{
		super.OnInit();
		Enable(false);
		m_pFactionManager = KSC_FactionManager.Cast(GetGame().GetFactionManager());
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnUpdate(ESystemPoint point)
	{
		super.OnUpdate(point);
		
		if (point == ESystemPoint.SimulatePhysics)
		{
			foreach (KSC_ParadropSystem_HeloContext heloContext : m_aHeloContexts)
			{
				heloContext.m_pHeloPhysics.SetVelocity(heloContext.m_vTargetDir * m_fSpeed);
				heloContext.m_pHeloPhysics.SetAngularVelocity(vector.Zero);
			}
		}
		else if (point == ESystemPoint.Frame)
		{
			HandleDespawning(GetWorld().GetPhysicsTimeSlice());
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Despawns owner when it moved the despawn distance
	protected void HandleDespawning(float timeSlice)
	{
		m_fDistanceCheckTimer += timeSlice;
		if (m_fDistanceCheckTimer < DISTANCE_CHECK_TIMEOUT)
			return;
		
		m_fDistanceCheckTimer = 0;
		
		for (int i = m_aHeloContexts.Count() - 1; i >= 0; --i)
		{
			KSC_ParadropSystem_HeloContext heloContext = m_aHeloContexts[i];
			
			if (vector.DistanceXZ(heloContext.m_vStartPos, heloContext.m_pHelo.GetOrigin()) < m_fDespawnDistance && !heloContext.m_bShouldSimulationStop)
				continue;
			
			Terminate(i, !heloContext.m_bShouldSimulationStop);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ScheduleParadrop(Faction faction, vector targetTransform[4], out Vehicle helo, out AIGroup crewGroup, out AIGroup passengerGroup)
	{
		array<ResourceName> entries = {};
		m_pFactionManager.GetFactionEntityListWithLabels(faction, EEntityCatalogType.VEHICLE, {EEditableEntityLabel.VEHICLE_HELICOPTER}, {EEditableEntityLabel.TRAIT_ARMED}, entries);
		if (entries.IsEmpty())
			return;
		
		vector startTransform[4];
		ComputeInitialAircraftTransform(targetTransform, startTransform);
		helo = Vehicle.Cast(KSC_GameTools.SpawnPrefab(entries.GetRandomElement(), startTransform));

		entries.Clear();
		m_pFactionManager.GetFactionEntityListWithLabel(faction, EEntityCatalogType.CHARACTER, EEditableEntityLabel.TRAIT_HELI_CREW, entries);
		if (entries.IsEmpty())
			m_pFactionManager.GetFactionEntityListWithLabel(faction, EEntityCatalogType.CHARACTER, EEditableEntityLabel.ROLE_RIFLEMAN, entries);
		
		SCR_ChimeraCharacter pilot = KSC_GameTools.SpawnCharacterPrefab(entries.GetRandomElement(), startTransform[3]);
		crewGroup = KSC_GroupHelper.GetGroup(pilot);
		SetPermanentLOD(crewGroup, 0);
		
		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(pilot.GetCompartmentAccessComponent());
		compartmentAccess.MoveInVehicle(helo, ECompartmentType.PILOT, true);
		
		HelicopterControllerComponent controller = HelicopterControllerComponent.Cast(helo.FindComponent(HelicopterControllerComponent));
		if (controller)
			controller.StartEngine();
		
		entries.Clear();
		m_pFactionManager.GetFactionEntityListWithLabel(faction, EEntityCatalogType.GROUP, EEditableEntityLabel.GROUPSIZE_LARGE, entries);
		passengerGroup = KSC_ParadropHelper.SpawnGroupInVehicle(helo, entries.GetRandomElement());
		SetPermanentLOD(passengerGroup, 0);
		
		// TO DO: Use frame event
		GetGame().GetCallqueue().CallLater(EjectPassengers,  m_fStartDistance / m_fSpeed * 1000 - 2.5 * KSC_ParadropHelper.PASSENGER_EJECT_DELAY_MS + Math.RandomIntInclusive(0, 1000), false, passengerGroup, targetTransform[3]);
		m_aHeloContexts.Insert(new KSC_ParadropSystem_HeloContext(helo, crewGroup));
		Enable(true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SetPermanentLOD(AIGroup group, int lod)
	{
		array<AIAgent> agents = {};
		group.GetAgents(agents);
		agents.Insert(group);
		
		foreach (AIAgent agent : agents)
		{
			agent.SetPermanentLOD(lod);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void EjectPassengers(AIGroup passengerGroup, vector targetPos)
	{
		if (!passengerGroup)
			return;
		
		KSC_ParadropHelper.EjectPassengers(KSC_GroupHelper.GetUnits(passengerGroup));
		// TO DO: Use landing event, as waypoint won't work when not everyone has landed yet
		GetGame().GetCallqueue().CallLater(AssignSAD, 60000, false, passengerGroup, targetPos);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void AssignSAD(AIGroup passengerGroup, vector targetPos)
	{
		if (passengerGroup)
			KSC_AITasks.SearchAndDestroy(passengerGroup, targetPos);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void Terminate(int contextIdx, bool deleteHelo)
	{
		Vehicle helo = m_aHeloContexts[contextIdx].m_pHelo;
		m_aHeloContexts.Remove(contextIdx);
		
		if (deleteHelo)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(helo);
		}
		else
		{
			HelicopterControllerComponent controller = HelicopterControllerComponent.Cast(helo.FindComponent(HelicopterControllerComponent));
			if (controller)
				controller.StopEngine();
		}
		
		if (m_aHeloContexts.IsEmpty())
			Enable(false);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ComputeInitialAircraftTransform(vector moduleTransform[4], out vector transform[4])
	{
		transform = moduleTransform;
		vector offset = m_fStartDistance * transform[2];
		transform[3] = transform[3] - offset;
		Math3D.AnglesToMatrix(Vector(offset.ToYaw(), 0, 0), transform);
		
		float maxTerrainY = 0;
		for (int i = 0; i < Math.Ceil(2 * m_fStartDistance / TERRAIN_Y_SCAN_STEP_SIZE); i++)
		{
			maxTerrainY = Math.Max(maxTerrainY, SCR_TerrainHelper.GetTerrainY(transform[3] + i * TERRAIN_Y_SCAN_STEP_SIZE * transform[2]));
		}
		
		transform[3][1] = transform[3][1] + maxTerrainY + m_fHeight;
	}
}

//------------------------------------------------------------------------------------------------
class KSC_ParadropSystem_HeloContext : Managed
{
	Vehicle m_pHelo;
	Physics m_pHeloPhysics;
	vector m_vStartPos;
	vector m_vTargetDir;
	AIGroup m_pCrewGroup;
	
	protected ref array<AIAgent> m_aAgents = {};
	protected SCR_CharacterControllerComponent m_pPilotController;
	protected ref array<SCR_RotorDamageManagerComponent> m_aRotorDamageManagers = {};

	bool m_bShouldSimulationStop = false;
	
	//------------------------------------------------------------------------------------------------
	void KSC_ParadropSystem_HeloContext(Vehicle helo, AIGroup crewGroup)
	{
		m_pHelo = helo;
		m_pHeloPhysics = helo.GetPhysics();
		m_pHeloPhysics.SetActive(ActiveState.ACTIVE);
		
		m_pCrewGroup = crewGroup;
		SCR_ChimeraCharacter pilot = SCR_ChimeraCharacter.Cast(m_pCrewGroup.GetLeaderEntity());
		m_pPilotController = SCR_CharacterControllerComponent.Cast(pilot.FindComponent(SCR_CharacterControllerComponent));
		m_pPilotController.m_OnLifeStateChanged.Insert(OnPilotLifeStateChanged);
		
		vector transform[4];
		m_pHelo.GetWorldTransform(transform);
		m_vStartPos = transform[3];
		m_vTargetDir = transform[2];
		
		IEntity child = m_pHelo.GetChildren();
		while (child)
		{
			SCR_RotorDamageManagerComponent damageManager = SCR_RotorDamageManagerComponent.Cast(child.FindComponent(SCR_RotorDamageManagerComponent));
			if (damageManager)
			{
				damageManager.GetOnDamageStateChanged().Insert(OnRotorDamageStateChanged);
				m_aRotorDamageManagers.Insert(damageManager);
			}
			
			child = child.GetSibling();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Terminate simulation when the life state of the pilot changes
	protected void OnPilotLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		m_bShouldSimulationStop = true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnRotorDamageStateChanged(EDamageState state)
	{
		if (state != EDamageState.DESTROYED)
			return;
		
		m_bShouldSimulationStop = true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_ParadropSystem_HeloContext()
	{
		if (m_pPilotController)
			m_pPilotController.m_OnLifeStateChanged.Remove(OnPilotLifeStateChanged);
		
		foreach (SCR_RotorDamageManagerComponent damageManager : m_aRotorDamageManagers)
		{
			damageManager.GetOnDamageStateChanged().Remove(OnRotorDamageStateChanged);
		}
	}
}
