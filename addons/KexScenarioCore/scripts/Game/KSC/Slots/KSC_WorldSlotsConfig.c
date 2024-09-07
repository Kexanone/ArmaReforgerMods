//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true)]
class KSC_WorldSlotsConfig
{
	[Attribute()]
	ref array<ref KSC_Location> m_aLocations;
	
	[Attribute()]
	ref KSC_BuildingSlotConfig m_pBuildingSlotConfig;
	
	[Attribute()]
	ref array<ref KSC_TerrainSlotConfig> m_mTerrainSlots;
	
	//------------------------------------------------------------------------------------------------
	map<EEditableEntityLabel, ref array<vector>> CompileTerrainSlotsInArea(KSC_AreaBase area)
	{
		vector mins, maxs;
		area.GetBoundBox(mins, maxs);
		
		map<EEditableEntityLabel, ref array<vector>> allSlots = new map<EEditableEntityLabel, ref array<vector>>();
		
		foreach (KSC_TerrainSlotConfig entry : m_mTerrainSlots)
		{
			array<vector> data = {};
			data.Reserve((maxs[0] - mins[0]) * (maxs[2] - mins[2]));
						
			for (int i = 0; i < entry.m_mMatrix.GetRowIcs().Count(); i++)
			{
				float x = entry.m_mMatrix.GetRowIcs()[i];
				if (x < mins[0] || x > maxs[0])
					continue;
				
				float z = entry.m_mMatrix.GetColIcs()[i];
				if (z < mins[2] || z > maxs[2])
					continue;
				
				vector v =  Vector(x, entry.m_mMatrix.GetData()[i], z);
				if (!area.IsPointInArea(v))
					continue;
				
				data.Insert(v);
			}
			
			if (!data.IsEmpty())
				allSlots[entry.m_eLabel] = data;
		}
		
		return allSlots;
	}
}
