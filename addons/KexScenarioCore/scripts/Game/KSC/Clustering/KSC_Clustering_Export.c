//------------------------------------------------------------------------------------------------
class KSC_Clustering_Exporter : Managed
{
	protected string m_sWorldName;
	protected ref array<typename> m_aTypes = {SCR_DestructibleBuildingEntity, Tree};
	protected ref map<typename, ref FileHandle> m_mStreams = new map<typename, ref FileHandle>();
	
	//------------------------------------------------------------------------------------------------
	void Run(array<typename> types = null)
	{
		m_sWorldName = FilePath.StripExtension(FilePath.StripPath(GetGame().GetWorldFile()));
		FileIO.MakeDirectory(m_sWorldName);
		ExportObjects(types);
		ExportTerrain();

	}
	
	//------------------------------------------------------------------------------------------------
	protected void ExportTerrain()
	{
		BaseWorld world = GetGame().GetWorld();
		vector mins, maxs;
		world.GetBoundBox(mins, maxs);
		int xMin = Math.Round(mins[0]);
		int xMax = Math.Round(maxs[0]);
		int zMin = Math.Round(mins[2]);
		int zMax = Math.Round(maxs[2]);
		array<int> idxListRoad = {}, idxListPlacement = {};
		
		for (int x = xMin; x <= xMax; x++)
		{
			for (int z = zMin; z <= zMax; z++)
			{
				vector pos = Vector(x, 0, z);
				pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
				
				if (KSC_TerrainHelper.SurfaceIsRoad(pos))
				{
					idxListRoad.Insert(x);
					idxListRoad.Insert(z);
				}
				
				if (IsPlacement(pos))
				{
					idxListPlacement.Insert(x);
					idxListPlacement.Insert(z);
				}
			}
		}
		
		PrintFormat("%1 %2 %3 %4 %5", xMin, xMax - xMin + 1, zMin, zMax - zMin + 1, idxListRoad.Count() / 2);
		FileHandle streamRoad = FileIO.OpenFile(m_sWorldName + "/" + "Road.bin", FileMode.WRITE);
		streamRoad.Write(xMin); streamRoad.Write(xMax - xMin + 1);
		streamRoad.Write(zMin); streamRoad.Write(zMax - zMin + 1);
		streamRoad.Write(idxListRoad.Count() / 2);
		
		foreach (int idx : idxListRoad)
		{
			streamRoad.Write(idx);
		}
		
		streamRoad.Close();
		
		FileHandle streamPlacement = FileIO.OpenFile(m_sWorldName + "/" + "Placement.bin", FileMode.WRITE);
		streamPlacement.Write(xMin); streamPlacement.Write(xMax - xMin + 1);
		streamPlacement.Write(zMin); streamPlacement.Write(zMax - zMin + 1);
		streamPlacement.Write(idxListPlacement.Count() / 2);
		
		foreach (int idx : idxListPlacement)
		{
			streamPlacement.Write(idx);
		}
		
		streamPlacement.Close();
	}
	
	static void loadRoadSlots(EEditableEntityLabel label, out array<int> xList, out array<int> zList, out array<float> dirList)
	{
		string path = KSC_WorldTools.GetBaseWorldName() + "/";
		
		switch (label)
		{
			case EEditableEntityLabel.SLOT_ROAD_SMALL: { path += "RoadSmall.bin"; break; }
			case EEditableEntityLabel.SLOT_ROAD_MEDIUM: { path += "RoadMedium.bin"; break; }
			case EEditableEntityLabel.SLOT_ROAD_LARGE: { path += "RoadLarge.bin"; break; }
			default: { return; }
		}
		
		FileHandle stream = FileIO.OpenFile(path, FileMode.READ);
		stream.Seek(16);
		int nElements;
		stream.Read(nElements, 4);
		Print(nElements);
		xList.Reserve(nElements);
		zList.Reserve(nElements);
		dirList.Reserve(nElements);
		
		for (int i = 0; i < nElements; i++)
		{
			int x, z;
			float value;
			  
			stream.Read(x, 4); xList.Insert(x);
			stream.Read(z, 4); zList.Insert(z);
			stream.Read(value, 4); dirList.Insert(value);
		}
	}
	
	static void loadFlatSlots(EEditableEntityLabel label, out array<int> xList, out array<int> zList)
	{
		string path = KSC_WorldTools.GetBaseWorldName() + "/";
		
		switch (label)
		{
			case EEditableEntityLabel.SLOT_FLAT_SMALL: { path += "FlatSmall.bin"; break; }
			case EEditableEntityLabel.SLOT_FLAT_MEDIUM: { path += "FlatMedium.bin"; break; }
			case EEditableEntityLabel.SLOT_FLAT_LARGE: { path += "FlatLarge.bin"; break; }
			default: { return; }
		}
		
		FileHandle stream = FileIO.OpenFile(path, FileMode.READ);
		stream.Seek(16);
		int nElements;
		stream.Read(nElements, 4);
		Print(nElements);
		xList.Reserve(nElements);
		zList.Reserve(nElements);
		
		for (int i = 0; i < nElements; i++)
		{
			int x, z;
			  
			stream.Read(x, 4); xList.Insert(x);
			stream.Read(z, 4); zList.Insert(z);
		}
	}
	
	/*
	protected void ExportTerrain(int xStep = 1, int zStep = 1)
	{
		BaseWorld world = GetGame().GetWorld();
		vector mins, maxs;
		world.GetBoundBox(mins, maxs);
		int xMin = Math.Round(mins[0]);
		int xMax = Math.Round(maxs[0]);
		int zMin = Math.Round(mins[2]);
		int zMax = Math.Round(maxs[2]);
		
		FileHandle streamRoad = FileIO.OpenFile(m_sWorldName + "/" + "Road.dat", FileMode.WRITE);
		FileHandle streamPlacement = FileIO.OpenFile(m_sWorldName + "/" + "Placement.dat", FileMode.WRITE);
		streamRoad.Write(string.Format("(%1,%2,%3|%4,%5,%6)\n", xMin, xStep, xMax, zMin, zStep, zMax));
		streamPlacement.Write(string.Format("(%1,%2,%3|%4,%5,%6)\n", xMin, xStep, xMax, zMin, zStep, zMax));
		
		for (int x = xMin; x <= xMax; x += xStep)
		{
			for (int z = zMin; z <= zMax; z += zStep)
			{
				vector pos = Vector(x, 0, z);
				pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
				ResourceName material = KSC_TerrainHelper.GetSurfaceMaterial(pos);
				streamRoad.Write(KSC_TerrainHelper.SurfaceIsRoad(pos).ToString(true));
				streamPlacement.Write(IsPlacement(pos).ToString(true));
			}
			
			streamRoad.Write("\n");
			streamPlacement.Write("\n");
		}
		
		streamRoad.Close();
		streamPlacement.Close();
	}
	*/
	
	//------------------------------------------------------------------------------------------------
	protected bool IsPlacement(vector pos)
	{
		if (KSC_TerrainHelper.SurfaceIsWater(pos))
			return false;
		
		if (!KSC_TerrainHelper.SurfaceIsFlat(pos))
			return false;
		
		if (KSC_TerrainHelper.SurfaceIsRoad(pos))
			return false;
		
		if (KSC_TerrainHelper.SurfaceIsRunway(pos))
			return false;
		
		return KSC_WorldTools.TraceCuboid(pos + Vector(0, 2, 0), Vector(1, 4, 1), TraceFlags.ENTS, Tree);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ExportObjects(array<typename> types)
	{
		if (types)
			m_aTypes = types;
		
		foreach (typename type : m_aTypes)
		{
			m_mStreams[type] = FileIO.OpenFile(m_sWorldName + "/" + type.ToString() + ".dat", FileMode.WRITE);
		}
		
		BaseWorld world = GetGame().GetWorld();
		vector mins, maxs;
		world.GetBoundBox(mins, maxs);
		world.QueryEntitiesByAABB(mins, maxs, QueryExportCallback);
		
		foreach (FileHandle stream : m_mStreams)
		{
			stream.Close();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryExportCallback(IEntity entity)
	{
		FileHandle stream;
		if (!m_mStreams.Find(entity.Type(), stream))
			return true;
		
		VObject vObject = entity.GetVObject();
		if (!vObject)
			return true;
		
		stream.Write(vObject.GetResourceName() + ";" + entity.GetOrigin().ToString() + "\n");
		return true;
	}
}

