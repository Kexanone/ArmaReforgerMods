//------------------------------------------------------------------------------------------------
class KSC_PolygonArea : KSC_AreaBase
{
	ref array<float> m_aPolygon;
	protected bool m_bBoundBoxComputed = false;
	protected vector m_vBoundBoxMins, m_vBoundBoxMaxs;
	
	//------------------------------------------------------------------------------------------------
	void KSC_PolygonArea(array<float> polygon)
	{
		m_aPolygon = polygon;
	}
	
	//------------------------------------------------------------------------------------------------
	static KSC_PolygonArea FromPoints(array<vector> points)
	{
		array<float> polygon = {};
		
		foreach (vector point : points)
		{
			polygon.Insert(point[0]);
			polygon.Insert(point[2]);
		}
		
		return new KSC_PolygonArea(polygon);
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
		if (!m_bBoundBoxComputed)
		{
			if (m_aPolygon.Count() < 2)
				return;
			
			GetGame().GetWorld().GetBoundBox(m_vBoundBoxMins, m_vBoundBoxMaxs);
			m_vBoundBoxMins[0] = m_aPolygon[0];
			m_vBoundBoxMaxs[0] = m_aPolygon[0];
			m_vBoundBoxMins[2] = m_aPolygon[1];
			m_vBoundBoxMaxs[2] = m_aPolygon[1];
					
			foreach (int i, float x : m_aPolygon)
			{
				if (i % 2 == 0)
				{
					// i is even
					m_vBoundBoxMins[0] = Math.Min(m_vBoundBoxMins[0], m_aPolygon[i]);
					m_vBoundBoxMaxs[0] = Math.Max(m_vBoundBoxMaxs[0], m_aPolygon[i]);
				}
				else
				{
					// i is odd
					m_vBoundBoxMins[2] = Math.Min(m_vBoundBoxMins[2], m_aPolygon[i]);
					m_vBoundBoxMaxs[2] = Math.Max(m_vBoundBoxMaxs[2], m_aPolygon[i]);
				}
			}
			
			m_bBoundBoxComputed = true;
		}
		
		// vector makes copies for assignments
		mins = m_vBoundBoxMins;
		maxs = m_vBoundBoxMaxs;
	}
}
