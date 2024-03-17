//------------------------------------------------------------------------------------------------
class KSC_BuildingPosQuery : Managed
{
	protected AIGroup m_pProbe;
	protected CoverManagerComponent m_pCoverManager;
	protected CoverQueryComponent m_pCoverQuery;
	protected ref CoverQueryProperties m_QueryProps;
	protected NavmeshWorldComponent m_pNavmesh;
	protected IEntity m_pBuilding;
	protected vector m_vLocalMins, m_vLocalMaxs;
	protected ref array<vector> m_aPositions, m_aDirections;
	protected bool m_bIsRunning = false;
	protected int m_iMaxPos;
	protected int m_iMaxAttempts;
	protected int m_iAttempt;
	protected vector m_vCurrentQueryPos;
	protected static const string GROUP_PREFAB_NAME = "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et";
	protected static const float QUERY_RADIUS = 50;
	protected static const float ATTEMPT_TIMEOUT = 0.05;
	
	//------------------------------------------------------------------------------------------------
	void KSC_BuildingPosQuery(IEntity building)
	{
		m_pBuilding = building;
		m_pBuilding.GetBounds(m_vLocalMins, m_vLocalMaxs);
		m_pProbe = KSC_GameTools.SpawnGroupPrefab(GROUP_PREFAB_NAME);
		
		AIPathfindingComponent pathfinding = AIPathfindingComponent.Cast(m_pProbe.FindComponent(AIPathfindingComponent));
		if (!pathfinding)
			return;
		
		m_pNavmesh = pathfinding.GetNavmeshComponent();
		m_pCoverQuery = CoverQueryComponent.Cast(m_pProbe.FindComponent(CoverQueryComponent));
		m_pCoverManager = CoverManagerComponent.Cast(GetGame().GetAIWorld().FindComponent(CoverManagerComponent));
		CreateCoverQueryProps();
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_BuildingPosQuery()
	{
		SCR_EntityHelper.DeleteEntityAndChildren(m_pProbe);
	}

	//------------------------------------------------------------------------------------------------
	protected void CreateCoverQueryProps()
	{
		m_QueryProps = new CoverQueryProperties();

		m_QueryProps.m_vSectorDir = vector.Zero;
		m_QueryProps.m_fQuerySectorAngleCosMin = -1.0;
		m_QueryProps.m_fSectorDistMin = 0;
		m_QueryProps.m_fSectorDistMax = QUERY_RADIUS;
		//m_QueryProps.m_fCoverHeightMin;
		//m_QueryProps.m_fCoverHeightMax = 10.0;
		m_QueryProps.m_fCoverToThreatAngleCosMin = -1.0;
		m_QueryProps.m_fScoreWeightDirection = 0;
		m_QueryProps.m_fScoreWeightDistance = 1.0;
		m_QueryProps.m_bCheckVisibility = false;
		m_QueryProps.m_bSelectHighestScore = false;
		m_QueryProps.m_iMaxCoversToCheck = SCR_CoverQueryComponent.MAX_COVERS_LOW_PRIORITY;
		m_QueryProps.m_fScoreWeightNavmeshRay = 50;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get random building positions
	//! maxPos is the maximum number of positions queried
	//! maxAttempts is the maximum number of attempts to find positions
	void RunRandomQuery(int maxPos, int maxAttempts = 1000)
	{
		m_iMaxPos = maxPos;
		m_iMaxAttempts = maxAttempts;
		m_bIsRunning = true;
		m_iAttempt = 0;
		m_aPositions = {};
		m_aDirections = {};
		m_vCurrentQueryPos = GetRandomPosInBounds();
		GetGame().GetCallqueue().CallLater(RandomQueryAttempt, ATTEMPT_TIMEOUT, true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void RandomQueryAttempt()
	{
		vector outPos, outDir;
		ECoverSearchState result = QueryPos(m_vCurrentQueryPos, outPos, outDir);
		switch (result)
		{
			case ECoverSearchState.SUCCESS:
			{	
				m_aPositions.Insert(outPos);
				m_aDirections.Insert(outDir);
				m_vCurrentQueryPos = GetRandomPosInBounds();
				break;
			};
			
			case ECoverSearchState.FAIL:
			{
				m_vCurrentQueryPos = GetRandomPosInBounds();
				break;
			};
		};
		
		m_iAttempt++;
		
		if (m_iAttempt >= m_iMaxAttempts || m_aPositions.Count() >= m_iMaxPos)
		{
			GetGame().GetCallqueue().Remove(RandomQueryAttempt);
			m_bIsRunning = false;
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected vector GetRandomPosInBounds()
	{
		vector localPos;
		for (int i = 0; i < 3; i++)
		{
			localPos[i] = Math.RandomFloatInclusive(m_vLocalMins[i], m_vLocalMaxs[i]);
		};
		
		return m_pBuilding.CoordToParent(localPos);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return true if a position was found
	protected ECoverSearchState QueryPos(vector queryPos, out vector outPos, out vector outDir)
	{
		// Make sure the navmesh tile is loaded
		if (m_pNavmesh.IsTileRequested(queryPos))
			return ECoverSearchState.RUNNING;
		
		if (!m_pNavmesh.IsTileLoaded(queryPos))
		{
			m_pNavmesh.LoadTileIn(queryPos);
			return ECoverSearchState.RUNNING;
		};
		
		// Get closest point on navmesh
		m_pNavmesh.GetReachablePoint(queryPos, QUERY_RADIUS, queryPos);
		
		m_QueryProps.m_vSectorPos = queryPos;
		m_QueryProps.m_vAgentPos = queryPos;
		m_QueryProps.m_vThreatPos = queryPos; // Threat pos is not provided here, it's a basic query in radius
		
		int tilex, tiley, coverId;
		ECoverSearchState result = m_pCoverQuery.GetBestCover("Soldiers", m_QueryProps, outPos, outDir, tilex, tiley, coverId);
		if (result != ECoverSearchState.SUCCESS)
			return result;
		
		outDir -= outPos;
		outDir[1] = 0;
		outDir.Normalize();
		m_pCoverManager.SetOccupiedCover(tilex, tiley, coverId, true);
		return ECoverSearchState.SUCCESS;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsRunning()
	{
		return m_bIsRunning;
	}
	
	//------------------------------------------------------------------------------------------------
	array<vector> GetPositions()
	{
		return m_aPositions;
	}
	
	//------------------------------------------------------------------------------------------------
	array<vector> GetDirections()
	{
		return m_aDirections;
	}
}
