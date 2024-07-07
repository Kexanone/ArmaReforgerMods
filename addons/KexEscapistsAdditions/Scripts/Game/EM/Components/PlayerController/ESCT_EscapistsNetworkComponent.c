//------------------------------------------------------------------------------------------------
//! Takes care of Escapists-specific server <> client communication and requests
modded class ESCT_EscapistsNetworkComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	void EM_ShowSpecialEventNotification(LocalizedString message)
	{
		Rpc(RpcDo_Owner_EM_ShowSpecialEventNotification, message);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_Owner_EM_ShowSpecialEventNotification(LocalizedString message)
	{
		ResourceName imageSet = "{12BA7AE37121F768}UI/Textures/Icons/icons_wrapperUI-96.imageset";
		string iconName = "warning";
		float scale = 2;
		ESCT_GameConfigComponent gameConfig = ESCT_GameConfigComponent.GetInstance();
		ESCT_EnemyFaction faction = gameConfig.GetOccupyingFaction();	
		string title = WidgetManager.Translate("#Escapists-Support_Generic_Title", faction.GetFactionName());
		title = string.Format("<color rgba='196,2,52,255'>%1</color>", title);
		LocalizedString resultSubtitle = message + "<br/><br/>" + string.Format("<image set='%1' name='%2' scale='%3'/>", imageSet, iconName, scale);
		SCR_PopUpNotification.GetInstance().PopupMsg(title, 8, sound: SCR_SoundEvent.SOUND_SIREN, text2: resultSubtitle);
	}
}
