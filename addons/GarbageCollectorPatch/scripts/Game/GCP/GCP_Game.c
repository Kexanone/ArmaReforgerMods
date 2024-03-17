/*
//------------------------------------------------------------------------------------------------
modded class ArmaReforgerScripted : ChimeraGame
{
	//------------------------------------------------------------------------------------------------
	override bool OnGameStart()
	{
		bool result = super.OnGameStart();
		
		if (!Replication.IsServer())
			return result;
		
		GetCallqueue().CallLater(GCP_System.Tick, GCP_System.GetTickTimeout() * 1000, true);
		return result;
	}
}
*/
