//------------------------------------------------------------------------------------------------
//! Implement mission rotations based on MissionRotation.conf
modded class SCR_BaseGameMode : BaseGameMode
{
	[Attribute(defvalue: "{15DE7F09D7319797}Configs/Mission/MissionRotation.conf", desc: "Conflig file that specifies the mission rotation")]
	protected ResourceName m_sKSC_MissionRotationConfig;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.ResourceNamePicker, desc: "Configurations for locations, terrain and building slots", params: "conf")]
	protected ResourceName m_sKSC_WorldSlotsConfigName;
	protected ref KSC_WorldSlotsConfig m_pKSC_WorldSlotsConfig;
	
	//------------------------------------------------------------------------------------------------
	void SCR_BaseGameMode(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode())
			return;
		
		m_pKSC_WorldSlotsConfig = SCR_ConfigHelperT<KSC_WorldSlotsConfig>.GetConfigObject(m_sKSC_WorldSlotsConfigName);
	}
	
	//------------------------------------------------------------------------------------------------
	KSC_WorldSlotsConfig KSC_GetWorldSlotsConfig()
	{
		return m_pKSC_WorldSlotsConfig;
	}
	
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