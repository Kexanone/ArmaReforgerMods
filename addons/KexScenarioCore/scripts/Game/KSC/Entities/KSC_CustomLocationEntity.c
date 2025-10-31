//------------------------------------------------------------------------------------------------
class KSC_CustomLocationEntityClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_CustomLocationEntity : GenericEntity
{
	protected static ref array<KSC_CustomLocationEntity> s_aInstances = {};
	
	//------------------------------------------------------------------------------------------------
	void KSC_CustomLocationEntity(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode())
			return;
		
		s_aInstances.Insert(this);
	}
	
	//------------------------------------------------------------------------------------------------
	static array<KSC_CustomLocationEntity> GetInstances()
	{
		return s_aInstances;
	}
	
	//------------------------------------------------------------------------------------------------
	KSC_Location GetLocationData()
	{
		KSC_Location instance = new KSC_Location();
		instance.m_vCenter = GetOrigin();
		instance.m_sName = KSC_WorldTools.GetLocationName(instance.m_vCenter);
		
		ShapeEntity shape = ShapeEntity.Cast(GetChildren());
		if (!shape)
			return null;
		
		array<vector> shapePoints = {};
		shape.GetPointsPositions(shapePoints);
		
		array<float> polygon = {};
		polygon.Reserve(shapePoints.Count());
		
		foreach (vector point : shapePoints)
		{
			polygon.Insert(point[0] + instance.m_vCenter[0]);
			polygon.Insert(point[2] + instance.m_vCenter[2]);
		}
		
		instance.m_Area = new KSC_PolygonArea(polygon);
		return instance;	
	}
	
	//------------------------------------------------------------------------------------------------
	void ~KSC_CustomLocationEntity()
	{
		s_aInstances.RemoveItem(this);
	}
}