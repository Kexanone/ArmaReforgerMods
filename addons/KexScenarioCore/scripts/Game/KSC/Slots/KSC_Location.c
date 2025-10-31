//------------------------------------------------------------------------------------------------
class KSC_Location : ScriptAndConfig
{
	[Attribute()]
	LocalizedString m_sName;
	
	[Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	EEditableEntityLabel m_eLabel;
	
	[Attribute()]
	vector m_vCenter;
	
	[Attribute()]
	protected ref array<float> m_aPolygon;
	
	ref KSC_PolygonArea m_Area;
	
	//------------------------------------------------------------------------------------------------
	void KSC_Location()
	{
		if (!GetGame().InPlayMode())
			return;
		
		if (m_vCenter && m_sName.IsEmpty())
			m_sName = KSC_WorldTools.GetLocationName(m_vCenter);
		
		if (m_aPolygon && !m_aPolygon.IsEmpty())
			m_Area = new KSC_PolygonArea(m_aPolygon);
	}
}
