//------------------------------------------------------------------------------------------------
class LTS_LoadLootAction : LTS_BaseLootTransferAction
{
	//------------------------------------------------------------------------------------------------
	override protected ScriptedInventoryStorageManagerComponent GetTargetStorageManager()
	{
		return ScriptedInventoryStorageManagerComponent.Cast(m_pNearestVehicle.FindComponent(SCR_VehicleInventoryStorageManagerComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected BaseInventoryStorageComponent GetTargetStorage()
	{
		return BaseInventoryStorageComponent.Cast(m_pNearestVehicle.FindComponent(UniversalInventoryStorageComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ScriptedInventoryStorageManagerComponent GetSourceStorageManager()
	{
		return ScriptedInventoryStorageManagerComponent.Cast(GetOwner().FindComponent(SCR_InventoryStorageManagerComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected array<IEntity> FindItemsToTransfer()
	{
		array<IEntity> items = super.FindItemsToTransfer();
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!ownerChar)
			return items; 
		
		SCR_CharacterControllerComponent ownerCharController = SCR_CharacterControllerComponent.Cast(ownerChar.GetCharacterController());
		if (!ownerCharController)
			return items;
		
		IEntity weapon = ownerCharController.LTS_GetDroppedWeapon();
		if (weapon)
			items.Insert(weapon);
		
		return items;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!super.CanBeShownScript(user))
			return false;
		
		SCR_ChimeraCharacter ownerChar = SCR_ChimeraCharacter.Cast(GetOwner());
		if (!ownerChar)
			return false; 
		
		SCR_CharacterControllerComponent ownerCharController = SCR_CharacterControllerComponent.Cast(ownerChar.GetCharacterController());
		if (!ownerCharController || ownerCharController.GetLifeState() == ECharacterLifeState.ALIVE)
			return false;
		
		return true;
	}
}
