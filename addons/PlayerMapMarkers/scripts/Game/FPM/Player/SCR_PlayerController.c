//------------------------------------------------------------------------------------------------
void FPM_OnControlledEntityChangedServer(int playerID, IEntity from, IEntity to);
typedef func FPM_OnControlledEntityChangedServer;
typedef ScriptInvokerBase<FPM_OnControlledEntityChangedServer> FPM_OnControlledEntityChangedServerInvoker;

//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController : PlayerController
{
	ref FPM_OnControlledEntityChangedServerInvoker m_FPM_OnControlledEntityChangedServer = new FPM_OnControlledEntityChangedServerInvoker();
	
	//------------------------------------------------------------------------------------------------
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		
		if (!Replication.IsRunning() || Replication.IsServer())
			m_FPM_OnControlledEntityChangedServer.Invoke(GetPlayerId(), from, to);
	}
}