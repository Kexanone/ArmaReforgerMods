//------------------------------------------------------------------------------------------------
class KSC_PolygonArea : KSC_AreaBase
{
	protected ref array<float> m_aPolygon;
	
	//------------------------------------------------------------------------------------------------
	void KSC_PolygonArea(array<float> polygon)
	{
		m_aPolygon = polygon;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsPointInArea(vector pos)
	{
		return Math2D.IsPointInPolygon(m_aPolygon, pos[0], pos[2]);
	}
	
	//------------------------------------------------------------------------------------------------
	override vector SamplePointInArea()
	{
		float x, z;
		SCR_Math2D.GetRandomPointInPolygon(m_aPolygon, x, z);		
		return Vector(x, 0, z);
	}
	
	//------------------------------------------------------------------------------------------------
	override void GetBoundBox(out vector mins, out vector maxs)
	{
		if (m_aPolygon.Count() < 2)
			return;
		
		GetGame().GetWorld().GetBoundBox(mins, maxs);
		mins[0] = m_aPolygon[0];
		maxs[0] = m_aPolygon[0];
		mins[2] = m_aPolygon[1];
		maxs[2] = m_aPolygon[1];
				
		foreach (int i, float x : m_aPolygon)
		{
			if (i % 2)
			{
				// i is even
				mins[0] = Math.Min(mins[0], m_aPolygon[i]);
				maxs[0] = Math.Max(maxs[0], m_aPolygon[i]);
			}
			else
			{
				// i is odd
				mins[2] = Math.Min(mins[2], m_aPolygon[i]);
				maxs[2] = Math.Max(maxs[2], m_aPolygon[i]);
			};
		};
	}
}