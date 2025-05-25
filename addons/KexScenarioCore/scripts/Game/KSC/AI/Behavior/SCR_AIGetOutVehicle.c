//------------------------------------------------------------------------------------------------
modded class SCR_AIGetOutVehicle : SCR_AIVehicleBehavior
{
	//----------------------------------------------------------------------------------
	override void OnActionSelected()
	{
		if (!m_Utility.m_ConfigComponent.m_bKSC_EnableGetOutVehicle)
			return Fail();
		
		super.OnActionSelected();
	}
}
