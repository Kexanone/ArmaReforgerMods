//------------------------------------------------------------------------------------------------
class KSC_SuppliesHelper
{
	//------------------------------------------------------------------------------------------------
	static void SetSupplyPercentage(notnull IEntity entity, float percentage)
	{
		SCR_ResourceComponent resource = SCR_ResourceComponent.Cast(entity.FindComponent(SCR_ResourceComponent));
		if (resource)
		{
			SCR_ResourceContainer container = resource.GetContainer(EResourceType.SUPPLIES);
			if (container)
				container.SetResourceValue(percentage * container.GetMaxResourceValue());
		}
		
		IEntity child = entity.GetChildren();
		while (child)
		{
			SetSupplyPercentage(child, percentage);
			child = child.GetSibling();
		}
	}
}
