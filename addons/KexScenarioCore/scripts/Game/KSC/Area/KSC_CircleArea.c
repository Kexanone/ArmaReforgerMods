class KSC_CircleArea : KSC_AreaBase
{
	protected vector m_vCenter;
	protected float m_fRadius;
	protected float m_fRadiusSq;
	
	//------------------------------------------------------------------------------------------------
	void KSC_CircleArea(vector center, float radius)
	{
		m_vCenter = center;
		SetRadius(radius);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		return vector.DistanceSqXZ(m_vCenter, pos) <= m_fRadiusSq;
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		return SCR_Math2D.GenerateRandomPointInRadius(0, m_fRadius, m_vCenter);
	}
	
	override void GetBoundBox(out vector mins, out vector maxs)
	{
		vector world_mins, world_maxs;
		GetGame().GetWorld().GetBoundBox(world_mins, world_maxs);
		vector offset = {m_fRadius, 0, m_fRadius};
		mins = m_vCenter - offset;
		mins[1] = world_mins[1];
		maxs = m_vCenter + offset;
		maxs[1] = world_maxs[1];
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRadius(float radius)
	{
		m_fRadius = radius;
		m_fRadiusSq = Math.Pow(radius, 2);
	}
}
