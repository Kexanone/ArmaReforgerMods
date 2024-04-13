//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class AF_TriggerAvalancheContextAction: SCR_BaseContextAction
{
	[Attribute(uiwidget: UIWidgets.ComboBox, desc: "Placing flags enabled upon creating an entity.", enums: ParamEnumArray.FromEnum(EEditorPlacingFlags))]
	protected EEditorPlacingFlags m_PlacingFlag;
	
	[Attribute(desc: "Active labels and white listed labels and label groups")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (hoveredEntity || cursorWorldPosition == vector.Zero)
			return false;
		
		return SCR_BaseGameMode.Cast(GetGame().GetGameMode()).AF_CanLaunchAvalanche(cursorWorldPosition);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBeShown(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		SCR_BaseGameMode.Cast(GetGame().GetGameMode()).AF_LaunchAvalanche(cursorWorldPosition);
	}
}
