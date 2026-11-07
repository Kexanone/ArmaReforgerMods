//------------------------------------------------------------------------------------------------
//! Add support for CLI param "kscSlotExport" to request slot exporter on game start
modded class ArmaReforgerScripted : ChimeraGame
{
	//------------------------------------------------------------------------------------------------
	override bool OnGameStart()
	{
		if (System.IsCLIParam("kscSlotExport") && Replication.IsServer())
		{
			KSC_SlotExport exporter = new KSC_SlotExport();
			exporter.Start();
		}
		
		return super.OnGameStart();
	}
}

//------------------------------------------------------------------------------------------------
//! Codex vibe-coded adapter for interfacing with KSC_Clustering_Exporter
class KSC_SlotExport : KSC_Clustering_Exporter
{
	protected bool m_Failed;
	protected static const string OUTPUT = "$profile:slot-export/";

	//------------------------------------------------------------------------------------------------
	void Start()
	{
		Print("KSC_EXPORT: starting " + GetGame().GetWorldFile());
		m_sWorldName = "$profile:slot-export";
		FileIO.MakeDirectory(m_sWorldName);

		array<ref array<float>> rectangles;
		if (System.IsCLIParam("kscSlotExportXMin") || System.IsCLIParam("kscSlotExportZMin") || System.IsCLIParam("kscSlotExportXMax") || System.IsCLIParam("kscSlotExportZMax"))
		{
			string xmin, zmin, xmax, zmax;
			if (!System.GetCLIParam("kscSlotExportXMin", xmin) || !System.GetCLIParam("kscSlotExportZMin", zmin) || !System.GetCLIParam("kscSlotExportXMax", xmax) || !System.GetCLIParam("kscSlotExportZMax", zmax))
			{
				Fail("Supply all four bounds");
				return;
			}
			if (xmin.ToInt() > xmax.ToInt() || zmin.ToInt() > zmax.ToInt())
			{
				Fail("Invalid bounds");
				return;
			}
			rectangles = {{xmin.ToFloat(), zmin.ToFloat(), xmax.ToFloat(), zmax.ToFloat()}};
		}
		else if (!GetGame().GetMapManager())
		{
			Fail("World has no map manager; supply explicit bounds");
			return;
		}

		// Run() resets the output folder. Invoke its stages with our profile path.
		ExportObjects(null);
		if (rectangles)
			ExportTerrain(rectangles);
		else
			ExportTerrain();
		Finish();
	}

	//------------------------------------------------------------------------------------------------
	protected int CountTerrainRecords(string name)
	{
		FileHandle stream = FileIO.OpenFile(OUTPUT + name, FileMode.READ);
		if (!stream)
		{
			Fail("Cannot read KSC terrain export: " + name);
			return 0;
		}
		int length = stream.GetLength();
		stream.Close();
		if (length < 20 || (length - 20) % 8 != 0)
		{
			Fail("Invalid KSC terrain export size: " + name);
			return 0;
		}
		return (length - 20) / 8;
	}

	//------------------------------------------------------------------------------------------------
	//! KSC owns object matching, resource extraction, formatting, and stream closure.
	protected int CountObjectRecords(string name)
	{
		FileHandle stream = FileIO.OpenFile(OUTPUT + name, FileMode.READ);
		if (!stream)
		{
			Fail("Cannot read KSC object export: " + name);
			return 0;
		}
		int count;
		string line;
		while (stream.ReadLine(line) >= 0)
		{
			if (!line.IsEmpty()) count++;
		}
		stream.Close();
		return count;
	}

	//------------------------------------------------------------------------------------------------
	protected void Fail(string message)
	{
		if (m_Failed) return;
		m_Failed = true;
		Print("KSC_EXPORT: FAILED: " + message, LogLevel.ERROR);
		FileHandle error = FileIO.OpenFile(OUTPUT + "failed.txt", FileMode.WRITE);
		if (error)
		{
			error.Write(message);
			error.Close();
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void Finish()
	{
		int roads = CountTerrainRecords("Road.bin");
		int placements = CountTerrainRecords("Placement.bin");
		int buildings = CountObjectRecords("SCR_DestructibleBuildingEntity.dat");
		int trees = CountObjectRecords("Tree.dat");
		if (m_Failed) return;
		FileHandle manifest = FileIO.OpenFile(OUTPUT + "complete.json", FileMode.WRITE);
		if (!manifest)
		{
			Fail("Cannot open completion manifest");
			return;
		}
		string json = string.Format("{\"schema\":1,\"roads\":%1,\"placements\":%2,\"buildings\":%3,\"trees\":%4}", roads, placements, buildings, trees);
		int written = manifest.Write(json);
		manifest.Close();
		if (written != json.Length())
		{
			Fail("Completion manifest write failed");
			return;
		}
		Print("KSC_EXPORT: COMPLETE");
	}
}
