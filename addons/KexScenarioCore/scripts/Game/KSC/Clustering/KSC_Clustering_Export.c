//------------------------------------------------------------------------------------------------
class KSC_Clustering_Exporter : Managed
{
	protected string m_sWorldName;
	protected ref array<typename> m_aTypes = {SCR_DestructibleBuildingEntity, Tree};
	protected ref map<typename, ref FileHandle> m_mStreams = new map<typename, ref FileHandle>();
	
	//------------------------------------------------------------------------------------------------
	void Run(array<typename> types = null, array<ref array<float>> rectangles = null)
	{
		m_sWorldName = FilePath.StripExtension(FilePath.StripPath(GetGame().GetWorldFile()));
		FileIO.MakeDirectory(m_sWorldName);
		ExportObjects(types);
		
		if (rectangles)
			ExportTerrain(rectangles);
		else
			ExportTerrain();
	}
	
	//------------------------------------------------------------------------------------------------
	void ExportTerrain()
	{
		BaseWorld world = GetGame().GetWorld();
		vector mins, maxs;
		world.GetBoundBox(mins, maxs);
		array<ref array<float>> rectangles = {{mins[0], mins[2], maxs[0], maxs[2]}};
		ExportTerrain(rectangles);
	}
	
	//------------------------------------------------------------------------------------------------
	void ExportTerrain(array<ref array<float>> rectangles)
	{
		map<KSC_Clustering_EExportType, ref FileHandle> m_mStreams = new map<KSC_Clustering_EExportType, ref FileHandle>();
		m_mStreams[KSC_Clustering_EExportType.ROAD] = FileIO.OpenFile(m_sWorldName + "/" + "Road.bin", FileMode.WRITE);
		m_mStreams[KSC_Clustering_EExportType.PLACEMENT] = FileIO.OpenFile(m_sWorldName + "/" + "Placement.bin", FileMode.WRITE);
		
		int xMin = int.MAX;
		int zMin = int.MAX;
		int xMax = int.MIN;
		int zMax = int.MIN;
		
		foreach (array<float> rectangle : rectangles)
		{
			int xMinRectangle = Math.Round(rectangle[0]);
			if (xMinRectangle < xMin)
				xMin = xMinRectangle;
			
			int zMinRectangle = Math.Round(rectangle[1]);
			if (zMinRectangle < zMin)
				zMin = zMinRectangle;
			
			int xMaxRectangle = Math.Round(rectangle[2]);
			if (xMaxRectangle > xMax)
				xMax = xMaxRectangle;
			
			int zMaxRectangle = Math.Round(rectangle[3]);
			if (zMaxRectangle > zMax)
				zMax = zMaxRectangle;
		}
		
		// Write header
		foreach (FileHandle stream : m_mStreams)
		{
			stream.Write(xMin); stream.Write(xMax - xMin + 1);
			stream.Write(zMin); stream.Write(zMax - zMin + 1);
			stream.Write(0);
		}

		foreach (array<float> rectangle : rectangles)
		{
			xMin = Math.Round(rectangle[0]);
			zMin = Math.Round(rectangle[1]);
			xMax = Math.Round(rectangle[2]);
			zMax = Math.Round(rectangle[3]);
			
			for (int x = xMin; x <= xMax; x++)
			{
				for (int z = zMin; z <= zMax; z++)
				{
					vector pos = Vector(x, 0, z);
					pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
					
					if (KSC_TerrainHelper.SurfaceIsRoad(pos))
					{
						m_mStreams[KSC_Clustering_EExportType.ROAD].Write(x);
						m_mStreams[KSC_Clustering_EExportType.ROAD].Write(z);
					}
					
					if (IsPlacement(pos))
					{
						m_mStreams[KSC_Clustering_EExportType.PLACEMENT].Write(x);
						m_mStreams[KSC_Clustering_EExportType.PLACEMENT].Write(z);
					}
				}
			}
		}
		
		foreach (FileHandle stream : m_mStreams)
		{
			stream.Close();
		}
	}
	
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

//------------------------------------------------------------------------------------------------
enum KSC_Clustering_EExportType
{
	ROAD,
	PLACEMENT
}
