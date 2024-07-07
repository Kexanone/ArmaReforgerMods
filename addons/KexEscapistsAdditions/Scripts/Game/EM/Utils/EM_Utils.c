//------------------------------------------------------------------------------------------------
class EM_Utils
{
	//------------------------------------------------------------------------------------------------
	static array<IEntity> GetPlayers(bool aliveOnly = false)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		array<int> playerIds = {};
		playerManager.GetAllPlayers(playerIds);
		
		if (playerIds.IsEmpty())
			return {};
		
		array<IEntity> players = {};
		foreach (int playerId : playerIds)
		{
			IEntity player = playerManager.GetPlayerControlledEntity(playerId);
			
			if (!player)
				continue;
			
			if (!aliveOnly || SCR_AIDamageHandling.IsAlive(player))
			{
				players.Insert(player);
			}
		}
		
		return players;
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity GetRandomPlayer(bool aliveOnly = false)
	{
		array<IEntity> players = GetPlayers(aliveOnly);
		
		if (players.IsEmpty())
			return null;
		
		return players.GetRandomElement();
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnEntity(ResourceName moduleName, vector pos)
	{
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = pos;
		Resource res = Resource.Load(moduleName);	
		return GetGame().SpawnEntityPrefab(res, null, params);
	};
}