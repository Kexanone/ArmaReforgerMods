//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true)]
class KSC_BuildingSlotConfig
{
	[Attribute()]
	ref array<ref KSC_BuildingSlotConfigEntry> m_aBuildingSlots;
}

//------------------------------------------------------------------------------------------------
class KSC_BuildingSlotConfigEntry : ScriptAndConfig
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.ResourceNamePicker, desc: "Xob of the building", params: "xob")]
	ResourceName m_sBuildingXobName;
	
	[Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	EEditableEntityLabel m_eLabel;
	
	[Attribute()]
	ref array<ref PointInfo> m_aUnitPositions;
}
