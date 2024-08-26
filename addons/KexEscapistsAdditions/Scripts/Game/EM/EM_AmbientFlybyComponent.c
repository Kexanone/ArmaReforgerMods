class EM_AmbientFlybyComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class EM_AmbientFlybyComponent : ScriptComponent
{
	[Attribute(defvalue: "30", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "6000", desc: "Distance from starting position for triggering despawn in meters")]
	protected float m_fDespawnDistance;
	
	protected Physics m_Physics;
	protected vector m_vVelocity;
	protected vector m_vStartPos;
	protected ref set<AIAgent> m_aAgents = new set<AIAgent>();
	
	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;

		SetEventMask(owner, EntityEvent.SIMULATE);
		m_Physics = owner.GetPhysics();
		m_Physics.SetActive(ActiveState.ACTIVE);
		
		vector transform[4];
		owner.GetWorldTransform(transform);
		m_vStartPos = transform[3];
		m_vVelocity = m_fSpeed * transform[2];
		
		GetGame().GetCallqueue().Call(DelayedInit, owner);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DelayedInit(IEntity owner)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(owner.FindComponent(SCR_BaseCompartmentManagerComponent));
		compartmentManager.GetOnDoneSpawningDefaultOccupants().Insert(OnCrewSpawned);
		compartmentManager.SpawnDefaultOccupants({ECompartmentType.PILOT,  ECompartmentType.TURRET});		
		HelicopterControllerComponent ctrl = HelicopterControllerComponent.Cast(owner.FindComponent(HelicopterControllerComponent));
		ctrl.StartEngine();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCrewSpawned(SCR_BaseCompartmentManagerComponent compartmentManager = null, array<IEntity> spawnedCharacters = null, bool wasCanceled = false)
	{
		foreach (IEntity member : spawnedCharacters)
		{
			AIComponent aiComponent = AIComponent.Cast(member.FindComponent(AIComponent));
			AIAgent agent = aiComponent.GetAIAgent();
			m_aAgents.Insert(agent);
			m_aAgents.Insert(agent.GetParentGroup());
		}
		
		foreach (AIAgent agent : m_aAgents)
		{
			agent.SetPermanentLOD(0);
		}
		
		GetGame().GetCallqueue().CallLater(OnCrewSpawnedDelayed, 100, false, compartmentManager, spawnedCharacters, wasCanceled);
		compartmentManager.GetOnDoneSpawningDefaultOccupants().Remove(OnCrewSpawned);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCrewSpawnedDelayed(SCR_BaseCompartmentManagerComponent compartmentManager = null, array<IEntity> spawnedCharacters = null, bool wasCanceled = false)
	{
		foreach (IEntity character : spawnedCharacters)
		{
			SCR_CompartmentAccessComponent compartmentAccess = 	SCR_CompartmentAccessComponent.Cast(character.FindComponent(SCR_CompartmentAccessComponent));
			if (compartmentAccess.GetCompartment() && compartmentAccess.GetCompartment().GetType() == ECompartmentType.PILOT)
			{
				SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
				characterController.m_OnLifeStateChanged.Insert(OnPilotLifeStateChanged);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnSimulate(IEntity owner, float timeSlice)
	{
		m_Physics.SetVelocity(m_vVelocity);
		HandleDespawning(owner, timeSlice);
	}
	
	protected const float DISTANCE_CHECK_TIMEOUT = 2;
	protected float m_fDistanceCheckTimer = 0;
	
	//------------------------------------------------------------------------------------------------
	//! Despawns owner when it moved the despawn distance
	protected void HandleDespawning(IEntity owner, float timeSlice)
	{
		m_fDistanceCheckTimer += timeSlice;
		if (m_fDistanceCheckTimer < m_fDistanceCheckTimer)
			return;
		
		m_fDistanceCheckTimer = 0;
		
		if (vector.DistanceXZ(m_vStartPos, owner.GetOrigin()) < m_fDespawnDistance)
			return;
		
		SCR_EntityHelper.DeleteEntityAndChildren(owner);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Terminate simulation when the life state of the pilot changes
	protected void OnPilotLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		foreach (AIAgent agent : m_aAgents)
		{
			agent.SetPermanentLOD(-1);
		}
		
		ClearEventMask(GetOwner(), EntityEvent.SIMULATE);
		HelicopterControllerComponent ctrl = HelicopterControllerComponent.Cast(GetOwner().FindComponent(HelicopterControllerComponent));
		ctrl.StopEngine();
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetDespawnDistance(float distance)
	{
		m_fDespawnDistance = distance;
	}
}
