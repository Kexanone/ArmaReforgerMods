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
}
