//------------------------------------------------------------------------------------------------
class EM_FlyByEvent : EM_SpecialEventBase
{
	[Attribute(defvalue: "3", desc: "Number of aircrafts to spawn")]
	protected int m_iMaxNumAircrafts;
	
	[Attribute(defvalue: "30", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "35", desc: "Minimum flying height relative to terrain in meters")]
	protected float m_fMinHeight;
	
	[Attribute(defvalue: "80", desc: "Flying height relative to terrain at target position in meters")]
	protected float m_fTargetHeight;
	
	[Attribute(defvalue: "2000", desc: "Starting distance from player in meters")]
	protected float m_fStartDistance;
	
	[Attribute(defvalue: "6000", desc: "Distance from starting position for triggering despawn in meters")]
	protected float m_fDespawnDistance;
	
	[Attribute(defvalue: "150", desc: "Maximum distance from the target")]
	protected float m_fMaxFlybyDistance;
	
	protected const float TERRAIN_Y_SCAN_STEP_SIZE = 100;
	
	protected vector m_vTargetPos;
	protected ref array<Vehicle> m_aAircrafts;
		
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		IEntity target = EM_Utils.GetRandomPlayer(true);
		if (!target)
			return;
		
		m_vTargetPos = target.GetOrigin();
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = m_vTargetPos;
		float angle =  Math.PI2 * Math.RandomFloat01();
		vector offset = m_fStartDistance * Vector(Math.Cos(angle), 0, Math.Sin(angle));
		params.Transform[3] = params.Transform[3] + SCR_Math2D.GenerateRandomPointInRadius(0, m_fMaxFlybyDistance, vector.Zero) - offset;
		Math3D.AnglesToMatrix(Vector(offset.ToYaw(), 0, 0), params.Transform);
		
		float maxTerrainY = 0;
		for (int i = 0; i < Math.Ceil(m_fDespawnDistance / TERRAIN_Y_SCAN_STEP_SIZE); i++)
		{
			maxTerrainY = Math.Max(maxTerrainY, SCR_TerrainHelper.GetTerrainY(params.Transform[3] + i * TERRAIN_Y_SCAN_STEP_SIZE * params.Transform[2]));
		}
		
		params.Transform[3][1] = Math.Max(maxTerrainY + m_fMinHeight, params.Transform[3][1] + m_fTargetHeight);
				
		ESCT_FactionManager factionManager = ESCT_FactionManager.Cast(GetGame().GetFactionManager());
		FactionKey factionKey = factionManager.GetEnemyFactionKey();	
		
		m_aAircrafts = {};
		
		int n = Math.RandomIntInclusive(1, m_iMaxNumAircrafts);
		
		for (int i = 0; i < n; i++)
		{
			Vehicle aircraft;
			
			if (factionKey == "USSR")
			{
				aircraft = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{59491940105545FE}Prefabs/Vehicles/Helicopters/Mi8MT/EM_Mi8MT_ambient.et"), null, params));
			}
			else if (factionKey == "US")
			{
				aircraft = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{1E55F72F67A2E60D}Prefabs/Vehicles/Helicopters/UH1H/EM_UH1H_ambient.et"), null, params));
			}
			else
			{
				PrintFormat("EM_FlyByEvent.Run: Faction not supported: %1", factionKey, level: LogLevel.WARNING);
				return;
			}
			
			EM_AmbientFlybyComponent flyby = EM_AmbientFlybyComponent.Cast(aircraft.FindComponent(EM_AmbientFlybyComponent));
			flyby.SetSpeed(m_fSpeed);
			flyby.SetDespawnDistance(m_fDespawnDistance);
			m_aAircrafts.Insert(aircraft);
			
			params.Transform[3] = params.Transform[3] + Math.RandomFloatInclusive(40, 60) * vector.FromYaw(Math.RandomFloatInclusive(-60, 60) - angle);
		}
	}
}
