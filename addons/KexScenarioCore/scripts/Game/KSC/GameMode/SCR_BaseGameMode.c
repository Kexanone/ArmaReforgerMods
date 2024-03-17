//------------------------------------------------------------------------------------------------
//! Implement mission rotations based on MissionRotation.conf
modded class SCR_BaseGameMode : BaseGameMode
{
	[Attribute(defvalue: "{15DE7F09D7319797}Configs/Mission/MissionRotation.conf", desc: "Conflig file that specifies the mission rotation")]
	protected ResourceName m_sKSC_MissionRotationConfig;
	
	//------------------------------------------------------------------------------------------------
	//! Same as SCR_BaseGameMode.OnGameModeEnd, but schedule change of scenario instead of restart
	override protected void OnGameModeEnd(SCR_GameModeEndData endData)
	{
		m_OnGameModeEnd.Invoke(endData);

		#ifdef GMSTATS
		if (IsGameModeStatisticsEnabled())
		{
			if (SCR_GameModeStatistics.IsRecording())
				SCR_GameModeStatistics.StopRecording();
		}
		#endif
		
		// Automatically restart the session on game mode end if enabled
		float reloadTime = GetAutoReloadDelay();
		if (reloadTime > 0)
			GetGame().GetCallqueue().CallLater(KSC_MissionRotation.ChangeToNextScenario, reloadTime * 1000.0, false);	
	}
}