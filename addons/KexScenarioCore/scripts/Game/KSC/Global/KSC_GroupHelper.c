//------------------------------------------------------------------------------------------------
class KSC_GroupHelper
{
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup GetGroup(ChimeraCharacter char)
	{
		if (EntityUtils.IsPlayer(char))
			return GetPlayerGroup(char);
		else
			return GetAIGroup(char);
	}
	
	//------------------------------------------------------------------------------------------------
	protected static SCR_AIGroup GetPlayerGroup(ChimeraCharacter char)
	{
		
		PlayerManager playerManager = GetGame().GetPlayerManager();

		if (!playerManager)
			return null;

		SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_GroupsManagerComponent));

		if (!groupManager)
			return null;

		int playerId = playerManager.GetPlayerIdFromControlledEntity(char);

		return groupManager.GetPlayerGroup(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	protected static SCR_AIGroup GetAIGroup(ChimeraCharacter char)
	{
		AIControlComponent aiControl = char.GetAIControlComponent();
		if (!aiControl)
			return null;
		
		AIAgent agent = aiControl.GetAIAgent();
		if (!agent)
			return null;
		
		return SCR_AIGroup.Cast(agent.GetParentGroup());
	}
	
	//------------------------------------------------------------------------------------------------
	static array<SCR_ChimeraCharacter> GetUnits(AIGroup group)
	{
		array<SCR_ChimeraCharacter> units = { };
		array<AIAgent> agents = { };
		group.GetAgents(agents);

		foreach (AIAgent agent : agents)
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(agent.GetControlledEntity());
			if (char)
				units.Insert(char);
		}

		return units;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Disabling AI LODs means that AI will always be in LOD0 independent of how far players are from them.
	//! Hence, they will not get frozen. This is especially useful for QRFs
	static void EnableAILODs(SCR_AIGroup group, bool enabled)
	{
		group.GetOnAgentAdded().Remove(OnAgentAddedDisableLODs);
		int lod = -1;

		if (!enabled)
		{
			lod = 0;
			group.GetOnAgentAdded().Insert(OnAgentAddedDisableLODs)

		};

		group.SetPermanentLOD(lod);
		array<AIAgent> agents = {};
		group.GetAgents(agents);

		foreach (AIAgent agent: agents)
		{
			agent.SetPermanentLOD(lod);
		}
	}

	//------------------------------------------------------------------------------------------------
	static protected void OnAgentAddedDisableLODs(AIAgent agent)
	{
		agent.SetPermanentLOD(0);
	}
}
