//-------------------------------------------------------------------------------
modded class PolylineArea: GenericEntity
{
	//------------------------------------------------------------------------------------------------
	KSC_PolygonArea KSC_GetPolygonArea()
	{
		return KSC_PolygonArea(m_PointsWorld);
	}
}
