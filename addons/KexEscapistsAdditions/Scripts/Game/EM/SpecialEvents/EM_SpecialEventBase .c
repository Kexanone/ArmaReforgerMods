//------------------------------------------------------------------------------------------------
class EM_SpecialEventBase : ScriptAndConfig
{
	[Attribute(desc: "Notification message")]
	protected LocalizedString m_sNotificationMessage;
	
	[Attribute(defvalue: "true", desc: "Notification message")]
	protected bool m_bEnabled;
		
	//------------------------------------------------------------------------------------------------
	void Run();
	
	//------------------------------------------------------------------------------------------------
	bool IsEnabled()
	{
		return m_bEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	LocalizedString GetNotificationMessage()
	{
		return m_sNotificationMessage;
	}
}
