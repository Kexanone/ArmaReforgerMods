//------------------------------------------------------------------------------------------------
//! Tools for spawning entities
class KSC_GameTools
{
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
		return AIGroup.Cast(SpawnPrefab(name, pos, rotation));
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