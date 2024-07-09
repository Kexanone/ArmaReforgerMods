//------------------------------------------------------------------------------------------------
class EM_FlyByEvent : EM_SpecialEventBase
{
	[Attribute(defvalue: "30", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "35", desc: "Flying height relative to terrain in meters")]
	protected float m_fHeight;
	
	[Attribute(defvalue: "2000", desc: "Starting distance from player in meters")]
	protected float m_fStartDistance;
	
	[Attribute(defvalue: "6000", desc: "Distance from starting position for triggering despawn in meters")]
	protected float m_fDespawnDistance;
	
	protected const float TERRAIN_Y_SCAN_STEP_SIZE = 100;
	
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		IEntity target = EM_Utils.GetRandomPlayer(true);
		if (!target)
			return;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		target.GetWorldTransform(params.Transform);
		float angle =  Math.PI2 * Math.RandomFloat01();
		vector offset = m_fStartDistance * Vector(Math.Cos(angle), 0, Math.Sin(angle));
		params.Transform[3] = params.Transform[3] - offset;
		Math3D.AnglesToMatrix(Vector(offset.ToYaw(), 0, 0), params.Transform);
		
		float maxTerrainY = 0;
		for (int i = 0; i < Math.Ceil(m_fDespawnDistance / TERRAIN_Y_SCAN_STEP_SIZE); i++)
		{
			maxTerrainY = Math.Max(maxTerrainY, SCR_TerrainHelper.GetTerrainY(params.Transform[3] + i * TERRAIN_Y_SCAN_STEP_SIZE * params.Transform[2]));
		}
		
		params.Transform[3][1] = params.Transform[3][1] + maxTerrainY + m_fHeight;
		
		FactionKey factionKey = ESCT_GameConfigComponent.GetInstance().GetOccupyingFaction().GetFactionKey();
		IEntity helo;
		if (factionKey == "USSR")
			helo = GetGame().SpawnEntityPrefab(Resource.Load("{59491940105545FE}Prefabs/Vehicles/Helicopters/Mi8MT/EM_Mi8MT_ambient.et"), null, params);
		else if (factionKey == "US")
			helo = GetGame().SpawnEntityPrefab(Resource.Load("{1E55F72F67A2E60D}Prefabs/Vehicles/Helicopters/UH1H/EM_UH1H_ambient.et"), null, params);
		
		EM_AmbientFlybyComponent flyby = EM_AmbientFlybyComponent.Cast(helo.FindComponent(EM_AmbientFlybyComponent));
		flyby.SetSpeed(m_fSpeed);
		flyby.SetDespawnDistance(m_fDespawnDistance);
	}
}
