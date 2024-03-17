//------------------------------------------------------------------------------------------------
class KSC_PlayerHelper
{
	//------------------------------------------------------------------------------------------------
	//! Returns all player entities
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
			};
		};
		
		return players;
	}
}