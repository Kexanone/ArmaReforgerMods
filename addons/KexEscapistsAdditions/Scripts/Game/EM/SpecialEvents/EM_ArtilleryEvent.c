//------------------------------------------------------------------------------------------------
class EM_ArtilleryEvent : EM_SpecialEventBase
{
	[Attribute("{16C019598C940565}Prefabs/Systems/Artillery/ArtilleryWrapper_Huge.et", UIWidgets.ResourcePickerThumbnail, "Prefab resource name of artillery round", "et")]
	protected ResourceName m_sRoundResName;
	
	[Attribute("{93FC2145F05A1EEE}Prefabs/Systems/Sound/sound_airraidsiren.et", UIWidgets.ResourcePickerThumbnail, "Siren module prefab name", "et")]
	protected ResourceName m_sSirenResName;
	
	[Attribute(defvalue: "200", desc: "Radius of spread in meters")]
	protected float m_fRadius;
	
	[Attribute(defvalue: "30", desc: "Rounds per minute")]
	protected int m_iRPM;
	
	[Attribute(defvalue: "18", desc: "Number of rounds")]
	protected int m_iNumberOfRounds;
	
	protected int m_iRoundsFired;
	protected IEntity m_pTarget;
	protected vector m_vCenter;
	private ref RandomGenerator m_RNG = new RandomGenerator();
	
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		m_pTarget = EM_Utils.GetRandomPlayer(true);
		
		if (!m_pTarget)
			return;
		
		m_vCenter = m_pTarget.GetOrigin();
				
		for (int i; i < 3; i++)
		{
			GetGame().GetCallqueue().CallLater(SpawnSiren, 3000 + i * m_RNG.RandInt(1000, 3000));
		}
		
		GetGame().GetCallqueue().CallLater(StartBarrage, 25000);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SpawnSiren()
	{
		// Update target position if target lives
		if (m_pTarget)
		{
			m_vCenter = m_pTarget.GetOrigin();
		}
		
		vector pos = m_RNG.GenerateRandomPointInRadius(1000, 1500, m_vCenter);
		pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
		IEntity siren = EM_Utils.SpawnEntity(m_sSirenResName, pos);
		GetGame().GetCallqueue().CallLater(SCR_EntityHelper.DeleteEntityAndChildren, 73100, false, siren);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void StartBarrage()
	{
		m_iRoundsFired = 0;
		GetGame().GetCallqueue().CallLater(SpawnRound, 60000 / m_iRPM, true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SpawnRound()
	{
		float radius = m_fRadius * Math.RandomFloat01();
		float angle = 360 * Math.RandomFloat01();
		vector pos = m_vCenter + radius * Vector(Math.Cos(angle), 0, Math.Sin(angle));
		EM_Utils.SpawnEntity(m_sRoundResName, pos);
		m_iRoundsFired++;
		
		if (m_iRoundsFired >= m_iNumberOfRounds)
		{
			GetGame().GetCallqueue().Remove(SpawnRound);
			OnCompleted();
		}
	}
}
