/*
//------------------------------------------------------------------------------------------------
modded class SCR_DamageManagerComponent : DamageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	override void OnDamageStateChanged(EDamageState state)
	{
		super.OnDamageStateChanged(state);

		if (state == EDamageState.DESTROYED)
		{
			GCP_System.Insert(GetOwner());
		}
	}
}
*/