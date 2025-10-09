//------------------------------------------------------------------------------------------------
//! Implement mission rotations based on MissionRotation.conf
modded class SCR_BaseGameMode : BaseGameMode
{
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
}