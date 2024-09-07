//------------------------------------------------------------------------------------------------
class KSC_TerrainSlotTools
{
	static ref map<EEditableEntityLabel, float> s_mSlotRadii;
	protected static SCR_BaseGameMode s_pGameMode;
	
	//------------------------------------------------------------------------------------------------
	protected static void Init()
	{
		s_pGameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		s_mSlotRadii = new map<EEditableEntityLabel, float>();
		s_mSlotRadii[EEditableEntityLabel.SLOT_FLAT_SMALL] = 7;
		s_mSlotRadii[EEditableEntityLabel.SLOT_FLAT_MEDIUM] = 14;
		s_mSlotRadii[EEditableEntityLabel.SLOT_FLAT_LARGE] = 21;
		s_mSlotRadii[EEditableEntityLabel.SLOT_ROAD_SMALL] = 7;
		s_mSlotRadii[EEditableEntityLabel.SLOT_ROAD_MEDIUM] = 14;
		s_mSlotRadii[EEditableEntityLabel.SLOT_ROAD_LARGE] = 21;
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnInRandomFlatSlot(ResourceName prefabName, inout map<EEditableEntityLabel, ref array<vector>> terrainSlots, EEditableEntityLabel slotLabel = EEditableEntityLabel.SLOT_FLAT_SMALL, vector centerPos = vector.Zero)
	{
		if (!s_pGameMode)
			Init();
		
		return SpawnInRandomSlot(prefabName, terrainSlots, slotLabel, doCutTrees: true, centerPos: centerPos, numHeadings: 4);
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnInRandomRoadSlot(ResourceName prefabName, inout map<EEditableEntityLabel, ref array<vector>> terrainSlots, EEditableEntityLabel slotLabel = EEditableEntityLabel.SLOT_ROAD_MEDIUM, vector centerPos = vector.Zero)
	{
		if (!s_pGameMode)
			Init();
		
		return SpawnInRandomSlot(prefabName, terrainSlots, slotLabel, doCutTrees: true, centerPos: centerPos, numHeadings: 2);
	}
	
	//------------------------------------------------------------------------------------------------
	protected static IEntity SpawnInRandomSlot(ResourceName prefabName, inout map<EEditableEntityLabel, ref array<vector>> terrainSlots, EEditableEntityLabel slotLabel, bool doCutTrees, vector centerPos, int numHeadings)
	{
		vector pos;
		if (!GetRandomTerrainSlot(pos, terrainSlots, slotLabel))
			return null;
		
		float dir;
		
		if (centerPos != vector.Zero)
		{
			float refDir = KSC_NumTools.WrapAngle((pos - centerPos).ToYaw());
			float minDiff = float.INFINITY;
			
			for (int i = 0; i < numHeadings; i++)
			{
				float otherDir = KSC_NumTools.WrapAngle(pos[1] + 360 * i / numHeadings);
				float diff = Math.AbsFloat(otherDir - refDir);
				
				if (diff < minDiff)
				{
					dir = otherDir;
					minDiff = diff;
				}
			}
		}
		else
		{
			dir = pos[1] + 360 * Math.RandomInt(0, numHeadings) / numHeadings;
		}
		
		pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
		IEntity structure = KSC_GameTools.SpawnStructurePrefab(prefabName, pos, dir);
		
		if (doCutTrees)
		{
			float radius;
			s_mSlotRadii.Find(slotLabel, radius);
			array<IEntity> trees = {};
			KSC_WorldTools.GetEntitiesByType(trees, Tree, KSC_CircleArea(pos, radius));
			
			if (!trees.IsEmpty())
			{
				array<vector> treePositions = {};
				treePositions.Reserve(trees.Count());
				foreach (IEntity tree : trees)
				{
					treePositions.Insert(tree.GetOrigin());
				}
			
				s_pGameMode.ACE_DeleteEntitiesAtPositionsGlobal(treePositions);	
			}
		}		

		return structure;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool GetRandomParkingSlot(out vector pos, out float rotation, inout map<EEditableEntityLabel, ref array<vector>> terrainSlots)
	{
		if (!s_pGameMode)
			Init();
		
		array<EEditableEntityLabel> labels = {EEditableEntityLabel.SLOT_ROAD_SMALL, EEditableEntityLabel.SLOT_ROAD_MEDIUM, EEditableEntityLabel.SLOT_ROAD_LARGE};
		Math.Randomize(-1);
		SCR_ArrayHelperT<EEditableEntityLabel>.Shuffle(labels);
		
		foreach (EEditableEntityLabel label : labels)
		{
			if (!GetRandomTerrainSlot(pos, terrainSlots, label))
				continue;
			
			rotation = pos[1] + 180 * Math.RandomIntInclusive(0, 1);
			pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
			
			float radius = 1.5;
			
			if (label == EEditableEntityLabel.SLOT_ROAD_LARGE)
			{
				radius = 3.5;
			}
			else if (label == EEditableEntityLabel.SLOT_ROAD_MEDIUM)
			{
				radius = 2.5;
			}
			
			pos += radius * Vector(Math.Cos(Math.DEG2RAD * rotation), 0, -Math.Sin(Math.DEG2RAD * rotation));
			return true;
		}
				
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool GetRandomTerrainSlot(out vector slot, inout map<EEditableEntityLabel, ref array<vector>> terrainSlots, EEditableEntityLabel slotLabel, bool doBlock = true)
	{
		if (!s_pGameMode)
			Init();
		
		array<vector> slots = {};
		if (!terrainSlots.Find(slotLabel, slots))
			return false;
		
		if (slots.IsEmpty())
			return false;
		
		slot = slots.GetRandomElement();
		
		if (doBlock)
			BlockSlots(terrainSlots, slot, s_mSlotRadii[slotLabel]);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static void BlockSlots(inout map<EEditableEntityLabel, ref array<vector>> terrainSlots, vector center, float radius)
	{
		if (!s_pGameMode)
			Init();
		
		foreach (EEditableEntityLabel label, array<vector> slots :  terrainSlots)
		{
			for (int i = slots.Count() - 1; i >= 0; i--)
			{
				if (vector.DistanceXZ(center, slots[i]) < radius + s_mSlotRadii[label])
					slots.Remove(i);
			}
		}
	}
}
