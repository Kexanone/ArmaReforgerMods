//------------------------------------------------------------------------------------------------
modded class SCR_GroupsManagerComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	//! Force update of remaining group members' marker streaming rules when someone leaves the group
	override void OnGroupPlayerRemoved(SCR_AIGroup group, int playerID)
	{
		super.OnGroupPlayerRemoved(group, playerID);
		
		if(IsProxy())
			return;
		
		UpdateMarkerStreamRulesForAllGroupMembers(group);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Force update of group members' marker streaming rules when someone joins the group
	override void OnGroupPlayerAdded(SCR_AIGroup group, int playerID)
	{
		super.OnGroupPlayerAdded(group, playerID);
		
		if (IsProxy())
			return;
		
		UpdateMarkerStreamRulesForAllGroupMembers(group);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateMarkerStreamRulesForAllGroupMembers(SCR_AIGroup group)
	{
		SCR_MapMarkerManagerComponent mapMarkerManager = SCR_MapMarkerManagerComponent.GetInstance();
		
		foreach (int playerId : group.GetPlayerIDs())
		{
			mapMarkerManager.SetStreamRulesForPlayer(playerId); 
		}
	}
}