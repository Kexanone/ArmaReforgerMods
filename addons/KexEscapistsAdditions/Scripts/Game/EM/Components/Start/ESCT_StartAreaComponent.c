//------------------------------------------------------------------------------------------------
modded class ESCT_StartAreaComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	override protected void ProvideFeedback()
	{
		super.ProvideFeedback();
		
		EM_SpecialEventsSystem eventSystem = EM_SpecialEventsSystem.GetInstance();
		if (eventSystem)
			eventSystem.Start();
	}
}
