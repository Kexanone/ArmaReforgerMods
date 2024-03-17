/*
//------------------------------------------------------------------------------------------------
modded class SCR_BaseDeployableSpawnPointComponent : SCR_BaseDeployableInventoryItemComponent
{
	//------------------------------------------------------------------------------------------------
	override void Deploy(IEntity userEntity = null)
	{
		super.Deploy(userEntity);
		GCP_System.Withdraw(GetOwner());
	}

	//------------------------------------------------------------------------------------------------
	override void Dismantle(IEntity userEntity = null)
	{
		super.Dismantle(userEntity);
		GCP_System.Insert(GetOwner());
	}
}
*/