//------------------------------------------------------------------------------------------------
//! Adds a method for triggering avalanches
modded class SCR_BaseGameMode : BaseGameMode
{
	[Attribute(defvalue: "{B10CF68991E9C4DE}Prefabs/Environment/Avalanche/AF_AvalancheFront.et", desc: "Resource name for avalanche front", category: "Avalanche")]
	protected ResourceName m_sAF_FrontEntityName;
	[Attribute(defvalue: "105", desc: "Within this radius of an old avalanche start point, no new avalanches can be triggered (in meters)", category: "Avalanche")]
	protected float m_fAF_BlacklistRadius;
	[Attribute(defvalue: "30", desc: "Minmum slope in degrees for a starting point", category: "Avalanche")]
	protected float m_fAF_SlopeAngle;
	[Attribute(defvalue: "13", desc: "Maximum number of fronts", category: "Avalanche")]
	protected int m_iAF_MaxNumFronts;
	[Attribute(defvalue: "7.5", desc: "Distance between two fronts in meters on the initial spawn grid", category: "Avalanche")]
	protected float m_fAF_InterfrontDistance;
	[Attribute(defvalue: "2", desc: "The height above the terrain in meters for the front's spawn point", category: "Avalanche")]
	protected float m_fAF_SpawnHeight;
	
	protected ref array<vector> m_aAF_PreviousCenters = {};

	//------------------------------------------------------------------------------------------------
	//! Launch an avalanche by spawning fronts on a line perpenticular to the gradient of the terrain
	void AF_LaunchAvalanche(vector center)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		Math3D.MatrixIdentity3(params.Transform);
		params.Transform[3] = center;
		
		if (!AF_CanLaunchAvalanche(center))
			return;
		
		m_aAF_PreviousCenters.Insert(center);
		
		// dir is the gradient projected onto the XZ plane
		vector dir = SCR_TerrainHelper.GetTerrainNormal(center);
		dir[1] = 0;
		dir.Normalize();
		// perp is perpendicular to dir and used together for our spawn point grid
		vector perp = Vector(dir[2], 0, -dir[0]);
							
		for (int i = 0; i < m_iAF_MaxNumFronts; i++)
		{
			// Create spawn positions alternating between left and right with increasing distance from the center
			// Example of spawn point grid with 13 fronts (o being the center and x the other points):
			//
			//  x x x x x x
			// x x x o x x x
			// VVVVVVVVVVVVV
			//
			params.Transform[3] = params.Transform[3] + m_fAF_InterfrontDistance * i * Math.Pow(-1, i) * perp - m_fAF_InterfrontDistance * Math.Sin(i * Math.PI/2) * dir;
			// Add m_fAF_SpawnHeight to y to ensure the boulders don't fall through the terrain
			SCR_TerrainHelper.SnapToTerrain(params.Transform);
			params.Transform[3][1] = params.Transform[3][1] + m_fAF_SpawnHeight;
			GetGame().SpawnEntityPrefab(Resource.Load(m_sAF_FrontEntityName), null, params);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Slope has to be at least 30° and the location needs to be far enough from previous positions
	protected bool AF_CanLaunchAvalanche(vector pos)
	{
		vector surfaceNormal = SCR_TerrainHelper.GetTerrainNormal(pos);
		if (Math.RAD2DEG * Math.Acos(vector.Dot(Vector(0, 1, 0), surfaceNormal)) < m_fAF_SlopeAngle)
			return false;
		
		foreach (vector oldPos : m_aAF_PreviousCenters)
		{
			if (vector.DistanceXZ(pos, oldPos) <= m_fAF_BlacklistRadius)
				return false;
		};
		
		return true;
	}
}