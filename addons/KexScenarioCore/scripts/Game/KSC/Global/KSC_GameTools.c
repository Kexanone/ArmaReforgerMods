//------------------------------------------------------------------------------------------------
//! Tools for spawning entities
class KSC_GameTools
{
	protected static EAISkill s_eAISkill = EAISkill.REGULAR;
	
	//------------------------------------------------------------------------------------------------
	static void SetAISkill(EAISkill skill)
	{
		s_eAISkill = skill;
	}
	
	//------------------------------------------------------------------------------------------------
	static EAISkill GetAISkill()
	{
		return s_eAISkill;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Spawn prefab with the given transform
	static IEntity SpawnPrefab(ResourceName name, vector transform[4])
	{
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = transform;
		return GetGame().SpawnEntityPrefab(Resource.Load(name), null, params);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Spawn prefab at the given position and with the given rotation in XZ plane
	static IEntity SpawnPrefab(ResourceName name, vector pos, float rotation = 0)
	{
		vector transform[4];
		GetTransformFromPosAndRot(transform, pos, rotation);
		return SpawnPrefab(name, transform);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get transform from the given position and rotation in XZ plane
	static void GetTransformFromPosAndRot(out vector transform[4], vector pos, float rotation)
	{
		Math3D.MatrixIdentity3(transform);
		Math3D.AnglesToMatrix(Vector(rotation, 0, 0), transform);
		transform[3] = pos;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Spawn a structure, align it to the terrain surface and rebuild navmesh
	static IEntity SpawnStructurePrefab(ResourceName name, vector pos,  float rotation = 0)
	{
		vector transform[4];
		GetTransformFromPosAndRot(transform, pos, rotation);
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		IEntity entity = SpawnPrefab(name, transform);
		
		SCR_AIWorld aiWorld = SCR_AIWorld.Cast(GetGame().GetAIWorld());
		if (aiWorld)
			aiWorld.RequestNavmeshRebuildEntity(entity);
		
		return entity;
	}
	
	//------------------------------------------------------------------------------------------------
	// Spawn vehicle and activate physics
	static Vehicle SpawnVehiclePrefab(ResourceName name, vector pos = vector.Zero, float rotation = 0)
	{
		vector transform[4];
		GetTransformFromPosAndRot(transform, pos, rotation);
		return SpawnVehiclePrefab(name, transform);
	}
	
	//------------------------------------------------------------------------------------------------
	// Spawn vehicle and activate physics
	static Vehicle SpawnVehiclePrefab(ResourceName name, vector transform[4])
	{
		Vehicle vehicle = Vehicle.Cast(SpawnPrefab(name,transform));
		
		Physics physics = vehicle.GetPhysics();
		if (physics)
			physics.SetActive(ActiveState.ACTIVE);		
		
		return vehicle;
	}
		
	//------------------------------------------------------------------------------------------------
	// Spawn a group prefab
	static AIGroup SpawnGroupPrefab(ResourceName name, vector pos = vector.Zero, float rotation = 0)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(SpawnPrefab(name, pos, rotation));
		KSC_SpawnGroupHelperComponent spawnGroupHelper = KSC_SpawnGroupHelperComponent.Cast(group.FindComponent(KSC_SpawnGroupHelperComponent));
		spawnGroupHelper.SetSkill(s_eAISkill);
		spawnGroupHelper.SetCenter(pos);
		spawnGroupHelper.Enable(true);
		return group;
	}
	
	protected static const string GROUP_BASE_PREFAB_NAME = "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et";
	
	//------------------------------------------------------------------------------------------------
	// Spawn a single character
	static SCR_ChimeraCharacter SpawnCharacterPrefab(ResourceName name, vector pos = vector.Zero, float rotation = 0)
	{
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(SpawnPrefab(name, pos, rotation));
		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(char.FindComponent(SCR_AICombatComponent));
		combatComponent.SetAISkill(s_eAISkill);
		
		SCR_AIGroup group = SCR_AIGroup.Cast(SpawnGroupPrefab(GROUP_BASE_PREFAB_NAME, pos, rotation));
		group.SetFaction(char.GetFaction());
		group.AddAIEntityToGroup(char);
		return char;
	}
	
	//------------------------------------------------------------------------------------------------
	// Spawn a waypoint prefab
	static AIWaypoint SpawnWaypointPrefab(ResourceName name, vector pos)
	{
		return AIWaypoint.Cast(SpawnPrefab(name, pos));
	}
	
	//------------------------------------------------------------------------------------------------
	// Spawn a spawn point prefab
	static SCR_SpawnPoint SpawnSpawnPointPrefab(ResourceName name, vector pos, float rotation = 0)
	{
		return SCR_SpawnPoint.Cast(SpawnPrefab(name, pos, rotation));
	}
}