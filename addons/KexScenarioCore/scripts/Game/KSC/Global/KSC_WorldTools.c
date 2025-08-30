//------------------------------------------------------------------------------------------------
class KSC_WorldTools
{
	protected static ref array<IEntity> s_aMatchedEntities;
	protected static typename s_tQueriedType;
	protected static ResourceName s_sQueriedPrefabName;
	protected static ref KSC_AreaBase s_QueriedArea;
	protected static bool s_bFirstOnly;
	protected static vector s_vSearchPos;
	protected static float m_fNearestDistanceSq;
	protected static const float NEAREST_SEARCH_DISTANCE = 0.01;
	
	//------------------------------------------------------------------------------------------------
	// Returns all entities in a given area of the specified type
	// If <type> is typename.Empty, any entity gets matched
	// If <area> is null, the entire world gets queried
	// If <firstOnly> is true, the search is terminated after the first match
	static void GetEntitiesByType(notnull out array<IEntity> entities, typename type = typename.Empty, KSC_AreaBase area = null, bool firstOnly = false)
	{
		s_aMatchedEntities = entities;
		s_tQueriedType = type;
		s_QueriedArea = area;
		s_bFirstOnly = firstOnly;
		
		vector mins, maxs;
		
		if (area)
			area.GetBoundBox(mins, maxs);
		else
			GetGame().GetWorld().GetBoundBox(mins, maxs);
		
		GetGame().GetWorld().QueryEntitiesByAABB(mins, maxs, QueryEntitiesInAreaByTypeCallback);
		s_aMatchedEntities = null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add entity to matches when it is in the target area and the correct type
	protected static bool QueryEntitiesInAreaByTypeCallback(IEntity entity)
	{
		if (s_QueriedArea && !s_QueriedArea.IsPointInArea(entity.GetOrigin()))
			return true;
		
		if (s_tQueriedType != typename.Empty && !entity.Type().IsInherited(s_tQueriedType))
			return true;
		
		s_aMatchedEntities.Insert(entity);
		return !s_bFirstOnly;
	}
	
	//------------------------------------------------------------------------------------------------
	// Returns all entities in a given area of the prefab name
	// If <prefabName> is ResourceName.Empty, any prefab entity gets matched
	// If <area> is null, the entire world gets queried
	// If <firstOnly> is true, the search is terminated after the first match
	static void GetEntitiesByPrefabName(notnull out array<IEntity> entities, ResourceName prefabName = ResourceName.Empty, KSC_AreaBase area = null, bool firstOnly = false)
	{
		s_aMatchedEntities = entities;
		s_sQueriedPrefabName = prefabName;
		s_QueriedArea = area;
		s_bFirstOnly = firstOnly;
		
		vector mins, maxs;
		
		if (area)
			area.GetBoundBox(mins, maxs);
		else
			GetGame().GetWorld().GetBoundBox(mins, maxs);
		
		GetGame().GetWorld().QueryEntitiesByAABB(mins, maxs, QueryEntitiesInAreaByPrefabNameCallback);
		s_aMatchedEntities = null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add entity to matches when it is in the target area and has the correct prefab name
	protected static bool QueryEntitiesInAreaByPrefabNameCallback(IEntity entity)
	{
		EntityPrefabData prefabData = entity.GetPrefabData();
		if (!prefabData)
			return true;
		
		if (s_QueriedArea && !s_QueriedArea.IsPointInArea(entity.GetOrigin()))
			return true;
		
		if (s_sQueriedPrefabName != ResourceName.Empty && prefabData.GetPrefabName() != s_sQueriedPrefabName)
			return true;
		
		s_aMatchedEntities.Insert(entity);
		return !s_bFirstOnly;
	}
	
	static IEntity GetNearestEntityByType(vector pos, typename type = typename.Empty)
	{
		s_vSearchPos = pos;
		s_tQueriedType = type;
		s_aMatchedEntities = {null};
		m_fNearestDistanceSq = Math.Pow(NEAREST_SEARCH_DISTANCE, 2);
		GetGame().GetWorld().QueryEntitiesBySphere(pos, NEAREST_SEARCH_DISTANCE, QueryNearestEntityByTypeCallback);
		IEntity result = s_aMatchedEntities[0];
		s_aMatchedEntities = null;
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add entity to matches when it is in the target area and the correct type
	protected static bool QueryNearestEntityByTypeCallback(IEntity entity)
	{
		if (s_tQueriedType != typename.Empty && !entity.Type().IsInherited(s_tQueriedType))
			return true;
		
		float distanceSq = vector.DistanceSq(s_vSearchPos, entity.GetOrigin());
		
		if (m_fNearestDistanceSq > distanceSq)
		{
			m_fNearestDistanceSq = distanceSq;
			s_aMatchedEntities[0] = entity;
		};

		return (m_fNearestDistanceSq != 0);
	}
	
	//------------------------------------------------------------------------------------------------
	static string GetBaseWorldName()
	{
		string worldName = FilePath.StripExtension(FilePath.StripPath(GetGame().GetWorldFile()));
		int start = 1 + worldName.LastIndexOf("_");
		
		if (start <= 0)
			return worldName;
		
		return worldName.Substring(start, worldName.Length() - start);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get the name of the nearest location
	static LocalizedString GetLocationName(vector pos)
	{
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		if (!core)
			return string.Empty;
		
		SCR_EditableEntityComponent nearestLocation = core.FindNearestEntity(pos, EEditableEntityType.COMMENT, EEditableEntityFlag.LOCAL);
		if (!nearestLocation)
			return string.Empty;
		
		return nearestLocation.GetDisplayName();
	}
	
	protected static typename s_tExcludedType;
	
	//------------------------------------------------------------------------------------------------
	static bool TraceCuboid(vector center, vector size, TraceFlags flags = TraceFlags.ENTS | TraceFlags.OCEAN, typename excludedType = typename.Empty, BaseWorld world = null)
	{
		if (!world)
			world = GetGame().GetWorld();
		
		s_tExcludedType = excludedType;
		
		TraceParam params = new TraceParam();
		params.Flags = flags;
		
		array<vector> dirs = {
			// Body diagonals
			size / 2,
			Vector(-size[0], size[1], size[2]) / 2,
			Vector(size[0], -size[1], size[2]) / 2,
			Vector(size[0], size[1], -size[2]) / 2,
			// Edge midpoints connecting diagonals
			Vector(size[0], size[1], 0) / 2,
			Vector(size[0], -size[1], 0) / 2,
			Vector(size[0], 0, size[2]) / 2,
			Vector(size[0], 0, -size[2]) / 2,
			Vector(0, size[1], size[2]) / 2,
			Vector(0, -size[1], size[2]) / 2,
			// Face center connecting segements
			Vector(size[0], 0, 0) / 2,
			Vector(0, size[1], 0) / 2,
			Vector(0, 0, size[2]) / 2
		};
		
		foreach (vector dir : dirs)
		{
			params.Start = center + dir;
			params.End = center - dir;
			if (world.TraceMove(params, TraceCuboidFilter) < 1)
				return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected static bool TraceCuboidFilter(notnull IEntity entity)
	{
		if (s_tExcludedType == typename.Empty)
			return true;
		
		return !entity.Type().IsInherited(s_tExcludedType);
	}
	
	//------------------------------------------------------------------------------------------------
	static bool IsPosEmpty(vector pos, float emptyRadius = 0.5, float emptyHeight = 2)
	{
		return SCR_WorldTools.TraceCylinder(pos + Vector(0, emptyHeight/2, 0), emptyRadius, emptyHeight);
	}
}