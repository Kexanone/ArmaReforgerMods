//------------------------------------------------------------------------------------------------
class KSC_MissionRotationClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_MissionRotation : GenericEntity
{
	private static ResourceName MISSION_ROTATION_CONFIG = "{15DE7F09D7319797}Configs/Mission/MissionRotation.conf";
	
	//------------------------------------------------------------------------------------------------
	protected void KSC_MissionRotation(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		SetEventMask(EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Automatic change to next scenario if this entity is present in the world (meant for dummy world)
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		KSC_MissionRotationHeader header = KSC_MissionRotationHeader.Cast(GetGame().GetMissionHeader());
		if (!header || !header.m_bSelectRandomFirstMission)
		{
			GetGame().GetCallqueue().Call(ChangeToNextScenario);
		}
		else
		{
			GetGame().GetCallqueue().Call(ChangeToRandomScenario);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Switch to a random scenario specified in MissionRotation.conf
	static void ChangeToRandomScenario()
	{
		if (!Replication.IsServer())
			return;
		
		KSC_MissionRotationConfig rotationConfig = SCR_ConfigHelperT<KSC_MissionRotationConfig>.GetConfigObject(MISSION_ROTATION_CONFIG);
		if (!rotationConfig || rotationConfig.m_aScenarioIds.IsEmpty())
		{
			GameStateTransitions.RequestScenarioRestart();
			return;
		};
		
		Math.Randomize(-1);
		int i = Math.RandomInt(0, rotationConfig.m_aScenarioIds.Count());
		ResourceName nextScenarioId = rotationConfig.m_aScenarioIds[i];
		GameStateTransitions.RequestScenarioChangeTransition(nextScenarioId, "");		
	}

	//------------------------------------------------------------------------------------------------
	//! Switch to the next scenario specified in MissionRotation.conf
	static void ChangeToNextScenario()
	{
		if (!Replication.IsServer())
			return;
		
		KSC_MissionRotationConfig rotationConfig = SCR_ConfigHelperT<KSC_MissionRotationConfig>.GetConfigObject(MISSION_ROTATION_CONFIG);
		if (!rotationConfig || rotationConfig.m_aScenarioIds.IsEmpty())
		{
			GameStateTransitions.RequestScenarioRestart();
			return;
		};
		
		int i = 0;
		
		MissionHeader header = GetGame().GetMissionHeader();
		if (header)
		{
			ResourceName currentScenarioId = header.GetHeaderResourceName();
			i = (rotationConfig.m_aScenarioIds.Find(currentScenarioId) + 1) % rotationConfig.m_aScenarioIds.Count();
		};
		
		ResourceName nextScenarioId = rotationConfig.m_aScenarioIds[i];
		GameStateTransitions.RequestScenarioChangeTransition(nextScenarioId, "");
	}
}