//------------------------------------------------------------------------------------------------
class EM_SpecialEventsSystem : GameSystem
{
	// -- TO DO: Make events an attribute
	[Attribute("10", UIWidgets.EditBox, "Minimum timeout for next special event in minutes")]
	protected float m_fMinTimeout;
	
	[Attribute("15", UIWidgets.EditBox, "Maximum timeout for next special event in minutes")]
	protected float m_fMaxTimeout;
	
	[Attribute(desc: "List of available events")]
	protected ref array<ref EM_SpecialEventBase> m_aSpecialEvents;
	
	//------------------------------------------------------------------------------------------------
	protected override void OnInit()
	{
		super.OnInit();
		
		// Remove disabled events
		for (int i = m_aSpecialEvents.Count() - 1; i >= 0; i--)
		{
			if (!m_aSpecialEvents[i].IsEnabled())
				m_aSpecialEvents.RemoveOrdered(i);
		}
		
		ESCT_EscapistsGameMode.GetGameMode().GetEscapistsManager().GetOnRunStart().Insert(ScheduleNextEvent);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Schedule next event
	protected void ScheduleNextEvent()
	{
		GetGame().GetCallqueue().CallLater(SelectEvent, Math.RandomIntInclusive(m_fMinTimeout*60000, m_fMaxTimeout*60000));
	}
	
	//------------------------------------------------------------------------------------------------
	void SelectEvent()
	{
		EM_SpecialEventBase evt = m_aSpecialEvents.GetRandomElement();
		
		LocalizedString message = evt.GetNotificationMessage();
		if (!message.IsEmpty())
			ShowNotification(message);
		
		evt.Run();
		
		ScheduleNextEvent();
	}
	
	//------------------------------------------------------------------------------------------------
	void ShowNotification(LocalizedString message)
	{
		PlayerManager plrManager = GetGame().GetPlayerManager();

		array<int> playerIds = {};
		plrManager.GetPlayers(playerIds);

		foreach (int id : playerIds)
		{
			PlayerController controller = plrManager.GetPlayerController(id);
			if (!id)
				continue;

			ESCT_EscapistsNetworkComponent networkComponent = ESCT_EscapistsNetworkComponent.Cast(controller.FindComponent(ESCT_EscapistsNetworkComponent));
			if (!networkComponent)
				continue;

			networkComponent.EM_ShowSpecialEventNotification(message);
		}
	}
}
