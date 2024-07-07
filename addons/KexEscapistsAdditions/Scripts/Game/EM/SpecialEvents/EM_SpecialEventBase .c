//------------------------------------------------------------------------------------------------
class EM_SpecialEventBase : ScriptAndConfig
{
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
}
