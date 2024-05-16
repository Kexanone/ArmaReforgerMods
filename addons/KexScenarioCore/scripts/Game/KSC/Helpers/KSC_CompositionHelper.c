//------------------------------------------------------------------------------------------------
class KSC_CompositionHelper
{
	//------------------------------------------------------------------------------------------------
	static array<IEntity> GetChildrenByXobSubstring(IEntity composition, string substr)
	{
		array<IEntity> matches = {};
		
		IEntity child = composition.GetChildren();
		while (child)
		{
			VObject vObject = child.GetVObject();
			if (vObject && vObject.GetResourceName().IndexOf(substr) >= 0)
				matches.Insert(child);
			
			child = child.GetSibling();
		}
		
		return matches;
	}
}