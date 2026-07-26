//------------------------------------------------------------------------------------------------
class EM_ParadropEvent : EM_FlyByEvent
{
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		super.Run();
		ESCT_FactionManager factionManager = ESCT_FactionManager.Cast(GetGame().GetFactionManager());
		FactionKey factionKey = factionManager.GetEnemyFactionKey();
				
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = m_vTargetPos;
		
		foreach (Vehicle aircraft : m_aAircrafts)
		{
			SCR_AIGroup group;
			
			if (factionKey == "USSR")
			{
				group = SCR_AIGroup.Cast(KSC_ParadropHelper.SpawnGroupInVehicle(aircraft, "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et"));
			}
			else if (factionKey == "US")
			{
				group = SCR_AIGroup.Cast(KSC_ParadropHelper.SpawnGroupInVehicle(aircraft, "{DDF3799FA1387848}Prefabs/Groups/BLUFOR/Group_US_RifleSquad.et"));
			}
			else if (factionKey == "RHS_AFRF")
			{
				group = SCR_AIGroup.Cast(KSC_ParadropHelper.SpawnGroupInVehicle(aircraft, "{D5BA898DF743F90B}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_3.0/Group_RHS_RF_MSV_VKPO_3.0_RifleSquad.et"));
			}
			else if (factionKey == "RHS_USAF")
			{
				group = SCR_AIGroup.Cast(KSC_ParadropHelper.SpawnGroupInVehicle(aircraft, "{F831DFB4A9B46152}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_RifleSquad.et"));
			}
			else
			{
				PrintFormat("EM_ParadropEvent.Run: Faction not supported: %1", factionKey, level: LogLevel.WARNING);
				return;
			}
			
			GetGame().GetCallqueue().CallLater(EjectPassengers,  m_fStartDistance / m_fSpeed * 1000 - 2.5 * KSC_ParadropHelper.PASSENGER_EJECT_DELAY_MS + Math.RandomIntInclusive(0, 1000), false, aircraft);
			AIWaypoint wp = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{EE9A99488B40628B}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_SearchAndDestroy.et"), null, params));
			wp.SetCompletionRadius(m_fMaxFlybyDistance);
			group.AddWaypoint(wp);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void EjectPassengers(Vehicle aircraft)
	{
		if (aircraft)
			KSC_ParadropHelper.EjectPassengers(aircraft);
	}
}
