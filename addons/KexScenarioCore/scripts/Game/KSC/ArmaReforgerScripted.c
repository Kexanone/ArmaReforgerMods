//! Main game instance.
//! Created when the game starts (not when a world starts!) and persists until the game is closed.
modded class ArmaReforgerScripted : ChimeraGame
{
	static MissionHeader s_pKSC_InitialMissionHeader;
	
	//------------------------------------------------------------------------------------------------
	//! This returns the initial mission header containing all the changes from the server config
	//! This method wor
	static MissionHeader KSC_GetInitialMissionHeader()
	{
		if (!s_pKSC_InitialMissionHeader)
			s_pKSC_InitialMissionHeader = GetGame().GetMissionHeader();
		
		return s_pKSC_InitialMissionHeader;
	}
}