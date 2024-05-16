//------------------------------------------------------------------------------------------------
class KSC_TerrainSlotConfig : ScriptAndConfig
{
	[Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	EEditableEntityLabel m_eLabel;
	
	[Attribute()]
	ref KSC_TerrainSlotMatrix m_mMatrix;
}

//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class KSC_TerrainSlotMatrix : KSC_CooMatrix<int>
{
	[Attribute()]
	protected ref array<int> m_iClusterLabel
	
	//------------------------------------------------------------------------------------------------
	array<int> GetClusterLabels()
	{
		return m_iClusterLabel;
	}
}
