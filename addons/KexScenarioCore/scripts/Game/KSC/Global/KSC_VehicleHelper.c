//------------------------------------------------------------------------------------------------
class KSC_VehicleHelper
{
	//------------------------------------------------------------------------------------------------
	static array<SCR_AIGroup> GetOccupantGroups(notnull IEntity vehicle)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return null;
		
		array<IEntity> occupants = {};
		compartmentManager.GetOccupants(occupants);
		
		array<SCR_AIGroup> groups = {};
		foreach (IEntity occupant : occupants)
		{
			ChimeraCharacter char = ChimeraCharacter.Cast(occupant);
			if (!char)
				continue;
			
			SCR_AIGroup group = KSC_GroupHelper.GetGroup(char);
			if (groups.Contains(group))
				continue;
			
			groups.Insert(group);
		}
		
		return groups;
	}
}
