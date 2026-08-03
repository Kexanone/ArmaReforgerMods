//------------------------------------------------------------------------------------------------
//! Remove private from helper methods
modded class SCR_BaseScoringSystemComponent
{
	//------------------------------------------------------------------------------------------------
	override protected int GetPlayerFactionIndex(int playerId)
	{
		return super.GetPlayerFactionIndex(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected Faction GetFactionByIndex(int factionIndex)
	{
		return super.GetFactionByIndex(factionIndex);
	}
}
