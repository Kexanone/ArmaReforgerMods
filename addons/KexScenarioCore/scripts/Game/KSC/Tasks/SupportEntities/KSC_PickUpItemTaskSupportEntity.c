//------------------------------------------------------------------------------------------------
class KSC_PickUpItemTaskSupportEntityClass : KSC_SubjectBaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_PickUpItemTask
class KSC_PickUpItemTaskSupportEntity : KSC_SubjectBaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	//! Get the subject's name
	override LocalizedString GetSubjectName(IEntity subject)
	{
		InventoryItemComponent item = InventoryItemComponent.Cast(subject.FindComponent(InventoryItemComponent));
		if (!item)
			return string.Empty;
		
		UIInfo info = item.GetUIInfo();
		if (!info)
			return string.Empty;
		
		return info.GetName();
		
	}
}