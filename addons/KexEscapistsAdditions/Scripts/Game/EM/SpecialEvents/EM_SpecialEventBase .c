//------------------------------------------------------------------------------------------------
class EM_SpecialEventBase : ScriptAndConfig
{
	[Attribute(desc: "Notification message")]
	protected LocalizedString m_sNotificationMessage;
	
	protected ref ScriptInvoker m_OnCompleted = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	void Run();
	
	//------------------------------------------------------------------------------------------------
	protected void OnCompleted()
	{
		m_OnCompleted.Invoke();
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnCompleted()
	{
		return m_OnCompleted;
	}
	
	LocalizedString GetNotificationMessage()
	{
		return m_sNotificationMessage;
	}
}
