//------------------------------------------------------------------------------------------------
class KSC_BuildingSlotConfig : ScriptAndConfig
{
	[Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	EEditableEntityLabel m_eLabel;
	
	[Attribute()]
	ref array<ref PointInfo> m_aUnitPositions;
}
