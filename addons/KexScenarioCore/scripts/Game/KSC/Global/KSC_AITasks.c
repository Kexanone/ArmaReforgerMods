//------------------------------------------------------------------------------------------------
class KSC_AITasks
{
	static int s_iMaxAttempts = int.MAX;
	
	//------------------------------------------------------------------------------------------------
	static void Patrol(AIGroup group, KSC_AreaBase area, int count = 5, float minDistance = 50)
	{
		AIFormationComponent formationHandler = AIFormationComponent.Cast(group.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation("Column");
		
		vector prevPos = vector.Zero;
		array<AIWaypoint> wpList = {};
		int i = 1;
		int attempts = 0;
		
		while (i <= count && attempts < s_iMaxAttempts)
		{
			vector pos = area.SamplePointInArea();
			pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
			if (vector.Distance(prevPos, pos) >= minDistance && !KSC_TerrainHelper.SurfaceIsWater(pos) &&  KSC_WorldTools.IsPosEmpty(pos))
			{
				AIWaypoint wp = KSC_GameTools.SpawnWaypointPrefab("{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et", pos);
				group.AddWaypoint(wp);
				wpList.Insert(wp);
				prevPos = pos;
				i++;
			}
			
			attempts++;
		}
		
		AIWaypointCycle wpCycle = AIWaypointCycle.Cast(KSC_GameTools.SpawnWaypointPrefab("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et", prevPos));
		wpCycle.SetWaypoints(wpList);
		group.AddWaypoint(wpCycle);
	}
	
	//------------------------------------------------------------------------------------------------
	static void Defend(AIGroup group, vector pos, float radius = 50)
	{
		AIWaypoint wp = KSC_GameTools.SpawnWaypointPrefab("{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", pos);
		wp.SetCompletionRadius(radius);
		group.AddWaypoint(wp);
	}
	
	//------------------------------------------------------------------------------------------------
	static void Defend(AIGroup group, KSC_AreaBase area, float radius = 50)
	{
		int attempts = 0;
		
		while (attempts < s_iMaxAttempts)
		{
			vector pos = area.SamplePointInArea();
			pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
			if (!KSC_TerrainHelper.SurfaceIsWater(pos) &&  KSC_WorldTools.IsPosEmpty(pos))
			{
				Defend(group, pos, radius);
				break;
			}
			
			attempts++;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	static void SearchAndDestroy(AIGroup group, vector pos, float radius = 30)
	{
		AIWaypoint wp = KSC_GameTools.SpawnWaypointPrefab("{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et", pos);
		wp.SetCompletionRadius(radius);
		group.AddWaypoint(wp);
	}
	
	//------------------------------------------------------------------------------------------------
	static void GetIn(IEntity vehicle, AIGroup group)
	{
		SCR_BoardingEntityWaypoint wp = SCR_BoardingEntityWaypoint.Cast(KSC_GameTools.SpawnWaypointPrefab("{712F4795CF8B91C7}Prefabs/AI/Waypoints/AIWaypoint_GetIn.et", vehicle.GetOrigin()));
		wp.SetEntity(vehicle);
		group.AddWaypoint(wp);
	}
	
	//------------------------------------------------------------------------------------------------
	//! To Do: Reset LOD after completion
	static void ArtillerySupport(IEntity vehicle, AIGroup group, vector pos, SCR_EAIArtilleryAmmoType ammoType, int shotCount)
	{
		KSC_GroupHelper.EnableAILODs(SCR_AIGroup.Cast(group), false);
		GetIn(vehicle, group);
		SCR_AIWaypointArtillerySupport wp = SCR_AIWaypointArtillerySupport.Cast(KSC_GameTools.SpawnWaypointPrefab("{C524700A27CFECDD}Prefabs/AI/Waypoints/AIWaypoint_ArtillerySupport.et", pos));
		group.AddWaypoint(wp);
		wp.SetAmmoType(ammoType);
		wp.SetTargetShotCount(shotCount);
		wp.SetActive(true);
	}
	
	//------------------------------------------------------------------------------------------------
	static void RemoveAll(AIGroup group)
	{
		array<AIWaypoint> wps = {};
		group.GetWaypoints(wps);
		
		foreach (AIWaypoint wp : wps)
		{
			group.RemoveWaypoint(wp);
		}
	}
}
