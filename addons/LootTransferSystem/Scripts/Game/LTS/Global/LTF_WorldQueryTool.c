//------------------------------------------------------------------------------------------------
class LTF_WorldQueryTool<IEntity T> : Managed
{
	protected ref array<T> m_aResults;
	protected T m_pResult;
	protected float m_fNearestDistance;
	protected vector m_vCenter;
	
	//------------------------------------------------------------------------------------------------
	array<T> FindAll(vector center, float radius)
	{
		m_aResults = {};
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, QueryAllCallback);
		return m_aResults;
	}
	
	//------------------------------------------------------------------------------------------------
	T FindNearest(vector center, float radius)
	{
		m_vCenter = center;
		m_pResult = null;
		m_fNearestDistance = float.INFINITY;
		GetGame().GetWorld().QueryEntitiesBySphere(center, radius, QueryNearestCallback);
		return m_pResult;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryAllCallback(IEntity entity)
	{
		T castedEntity = T.Cast(entity);
		if (castedEntity)
			m_aResults.Insert(castedEntity);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryNearestCallback(IEntity entity)
	{
		T castedEntity = T.Cast(entity);
		if (!castedEntity)
			return true;
		
		float distance = vector.Distance(m_vCenter, entity.GetOrigin());
		if (distance >= m_fNearestDistance)
			return true;
		
		m_pResult = castedEntity;
		m_fNearestDistance = distance;		
		return true;
	}
}
