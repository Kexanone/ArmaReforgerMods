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
		
		// Attach event handlers
		foreach (EM_SpecialEventBase e : m_aSpecialEvents)
		{
			e.GetOnCompleted().Insert(OnEventCompleted);
		}
		
		OnEventCompleted();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Schedule next event
	protected void OnEventCompleted()
	{
		GetGame().GetCallqueue().CallLater(SelectEvent, Math.RandomIntInclusive(m_fMinTimeout*60000, m_fMaxTimeout*60000));
	}
	
	//------------------------------------------------------------------------------------------------
	void SelectEvent()
	{
		m_aSpecialEvents.GetRandomElement().Run();
	}
}
