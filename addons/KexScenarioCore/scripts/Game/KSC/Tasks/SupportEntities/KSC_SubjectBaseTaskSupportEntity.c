//------------------------------------------------------------------------------------------------
class KSC_SubjectBaseTaskSupportEntityClass : KSC_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_SubjectBaseTask
class KSC_SubjectBaseTaskSupportEntity : KSC_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	SCR_BaseTask CreateTask(Faction targetFaction, IEntity subject,  string formatParam2 = "", string formatParam3 = "")
	{
		KSC_SubjectBaseTask task = KSC_SubjectBaseTask.Cast(super.CreateTask(targetFaction, subject.GetOrigin(), GetSubjectName(subject), formatParam2, formatParam3));
		task.SetSubject(subject);
		return task;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get the subject's name
	LocalizedString GetSubjectName(IEntity subject)
	{
		SCR_EditableEntityComponent edit = SCR_EditableEntityComponent.Cast(subject.FindComponent(SCR_EditableEntityComponent));
		if (!edit)
			return string.Empty;
		
		return edit.GetDisplayName();
		
	}
}