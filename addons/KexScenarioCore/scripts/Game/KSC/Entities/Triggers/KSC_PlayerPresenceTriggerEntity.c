//------------------------------------------------------------------------------------------------
class KSC_PlayerPresenceTriggerEntityClass : SCR_BaseTriggerEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_PlayerPresenceTriggerEntity : SCR_BaseTriggerEntity
{
	[Attribute(defvalue: "true", desc: "Excludes aircrafts from activating the trigger when true")]
	protected bool m_bIgnoreAircrafts;
	
	//------------------------------------------------------------------------------------------------
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!IsAlive(ent))
			return false;
		
		if (EntityUtils.IsPlayer(ent))
			return true;
		
		SCR_EditableVehicleComponent vehicle = SCR_EditableVehicleComponent.Cast(ent.FindComponent(SCR_EditableVehicleComponent));
		if (!vehicle)
			return false;
		
		if (m_bIgnoreAircrafts)
		{
			SCR_EditableVehicleUIInfo info = SCR_EditableVehicleUIInfo.Cast(vehicle.GetInfo());
			if (!info || info.HasEntityLabel(EEditableEntityLabel.VEHICLE_HELICOPTER) || info.HasEntityLabel(EEditableEntityLabel.VEHICLE_AIRPLANE))
				return false;
		}
		
		SCR_BaseCompartmentManagerComponent compartmentManager = vehicle.GetCompartmentManager();
		if (!compartmentManager)
			return false;
		
		array<IEntity> occupants = {};
		compartmentManager.GetOccupants(occupants);
		
		foreach (IEntity occupant : occupants)
		{
			if (EntityUtils.IsPlayer(occupant))
				return true;
		}
		
		return false;
	}
}
