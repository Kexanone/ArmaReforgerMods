//------------------------------------------------------------------------------------------------
modded class SCR_CampaignBuildingCompositionComponent : ScriptComponent
{
	protected static ref ScriptInvokerEntity s_KSC_OnCompositionSpawnedServer;
	
	//------------------------------------------------------------------------------------------------
	static ScriptInvokerEntity KSC_GetOnCompositionSpawnedServer()
	{
		if (!s_KSC_OnCompositionSpawnedServer)
			s_KSC_OnCompositionSpawnedServer = new ScriptInvokerEntity();
		
		return s_KSC_OnCompositionSpawnedServer;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void SetIsCompositionSpawned()
	{
		super.SetIsCompositionSpawned();
		
		if (Replication.IsServer() && s_KSC_OnCompositionSpawnedServer)
			s_KSC_OnCompositionSpawnedServer.Invoke(GetOwner());
	}
}
