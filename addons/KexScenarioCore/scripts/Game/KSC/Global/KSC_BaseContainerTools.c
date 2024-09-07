//------------------------------------------------------------------------------------------------
class KSC_BaseContainerTools
{
	//------------------------------------------------------------------------------------------------
	static LocalizedString GetDisplayName(BaseContainer container)
	{
		IEntitySource entitySource = container.ToEntitySource();
		if (!entitySource)
			return "";
		
		IEntityComponentSource editableEntitySource = SCR_EditableEntityComponentClass.GetEditableEntitySource(entitySource);
		if (!editableEntitySource)
			return "";
		
		SCR_EditableEntityUIInfo editableEntityUiInfo = SCR_EditableEntityComponentClass.GetInfo(editableEntitySource);
		if (!editableEntityUiInfo)
			return "";
		
		return editableEntityUiInfo.GetName();
	}
}
