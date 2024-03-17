//------------------------------------------------------------------------------------------------
modded class SCR_BaseGameMode : BaseGameMode
{
	//------------------------------------------------------------------------------------------------
	//! Initialize SA handling for players
	override void OnPlayerSpawnFinalize_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity)
	{
		super.OnPlayerSpawnFinalize_S(requestComponent, handlerComponent, data, entity);
		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(entity.FindComponent(SCR_CharacterDamageManagerComponent));
		if (damageManager)
			damageManager.SA_Initialize(entity);
	}
}