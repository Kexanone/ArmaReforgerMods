//------------------------------------------------------------------------------------------------
class EM_SpecialEventsSystem : GameSystem
{
	[Attribute("10", UIWidgets.EditBox, "Minimum timeout for next special event in minutes")]
	protected float m_fMinTimeout;
	
	[Attribute("15", UIWidgets.EditBox, "Maximum timeout for next special event in minutes")]
	protected float m_fMaxTimeout;
	
	[Attribute(desc: "List of available events")]
	protected ref array<ref EM_SpecialEventBase> m_aSpecialEvents;

	//------------------------------------------------------------------------------------------------
	static EM_SpecialEventsSystem GetInstance()
	{
		ChimeraWorld world = GetGame().GetWorld();
		return EM_SpecialEventsSystem.Cast(world.FindSystem(EM_SpecialEventsSystem));
	}
	
	//------------------------------------------------------------------------------------------------
	override static void InitInfo(WorldSystemInfo outInfo)
	{
		outInfo.SetAbstract(false)
			.SetUnique(true)
			.SetLocation(WorldSystemLocation.Server)
			.AddPoint(WorldSystemPoint.EntitiesInitialized);
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnInit()
	{
		// Remove disabled events
		for (int i = m_aSpecialEvents.Count() - 1; i >= 0; i--)
		{
			if (!m_aSpecialEvents[i].IsEnabled())
				m_aSpecialEvents.RemoveOrdered(i);
		}

		PrintFormat("[Kex Escapists Additions] [%1] Special events initialized.", Type().ToString());
	}
	
	//------------------------------------------------------------------------------------------------
	void Start()
	{
		ScheduleNextEvent();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Schedule next event
	protected void ScheduleNextEvent()
	{
		float delay = Math.RandomFloatInclusive(m_fMinTimeout, m_fMaxTimeout);
		PrintFormat("[Kex Escapists Additions] [%1] Next event in %2 minutes.", Type().ToString(), delay);
		GetGame().GetCallqueue().CallLater(SelectEvent, 60000 * delay);
	}
	
	//------------------------------------------------------------------------------------------------
	void SelectEvent()
	{
		EM_SpecialEventBase evt = m_aSpecialEvents.GetRandomElement();
		
		LocalizedString message = evt.GetNotificationMessage();
		if (!message.IsEmpty())
			ShowNotification(message);
		
		PrintFormat("[Kex Escapists Additions] [%1] Running %2.", Type().ToString(), evt.Type().ToString());
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
