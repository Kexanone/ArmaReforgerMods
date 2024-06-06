//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true)]
class KSC_BuildingSlotConfig
{
	[Attribute()]
	ref array<ref KSC_BuildingSlotConfigEntry> m_aBuildingSlots;
}

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), KSC_BuildingSlotConfigEntryTitle()]
class KSC_BuildingSlotConfigEntry
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.ResourceNamePicker, desc: "Xob of the building", params: "xob")]
	ResourceName m_sBuildingXobName;
	
	[Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	EEditableEntityLabel m_eLabel;
	
	[Attribute()]
	ref array<ref PointInfo> m_aUnitPositions;
}

//------------------------------------------------------------------------------------------------
class KSC_BuildingSlotConfigEntryTitle : BaseContainerCustomTitle
{
	//------------------------------------------------------------------------------------------------
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{			
		ResourceName xobName;
		source.Get("m_sBuildingXobName", xobName);
		EEditableEntityLabel label;
		source.Get("m_eLabel", label);
		title = string.Format("%1 (%2)", FilePath.StripPath(FilePath.StripExtension(xobName)), SCR_Enum.GetEnumName(EEditableEntityLabel, label)); 	
		return true;
	}
}