//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_ContainerActionTitle()]
modded class SCR_ScenarioFrameworkActionShowWidget : SCR_ScenarioFrameworkActionBase
{
	//------------------------------------------------------------------------------------------------
	void KSC_SetLines(array<string> lines)
	{
		m_aLines = lines;
	}
}
