//------------------------------------------------------------------------------------------------
class KSC_AreaTriggerTaskSupportEntityClass: KSC_BaseTaskSupportEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Support entity for KSC_BaseTask that are associated with an area trigger
class KSC_AreaTriggerTaskSupportEntity : KSC_BaseTaskSupportEntity
{
	[Attribute(desc: "Prefab name of the trigger")]
	protected ResourceName m_sTriggerPrefabName;
	
	//------------------------------------------------------------------------------------------------
	SCR_BaseTask CreateTask(Faction targetFaction, vector pos, float radius = 50, string formatParam1 = "", string formatParam2 = "", string formatParam3 = "")
	{
		if (formatParam1.IsEmpty())
			formatParam1 = KSC_WorldTools.GetLocationName(pos);
		
		KSC_AreaTriggerTask task = KSC_AreaTriggerTask.Cast(super.CreateTask(targetFaction, pos, formatParam1, formatParam2, formatParam3));
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = pos;
		SCR_BaseTriggerEntity trigger  = SCR_BaseTriggerEntity.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(m_sTriggerPrefabName), GetWorld(), params));
		trigger.SetSphereRadius(radius);
		trigger.EnablePeriodicQueries(false);
		task.SetTrigger(trigger);
		// Start of queries has to be delayed, otherwise the trigger gets completed before the AI spawns
		GetGame().GetCallqueue().CallLater(trigger.EnablePeriodicQueries, 10000, false, true);
		return task;
	}
}
