//------------------------------------------------------------------------------------------------
modded class SCR_EditableEntityComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	//! Improve vanilla destroy by using Kill method
	override bool Destroy()
	{
		if (IsServer())
		{			
			if (!IsDestroyed() && CanDestroy())
			{
				SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(m_Owner.FindComponent(SCR_DamageManagerComponent));
				damageManager.Kill(Instigator.CreateInstigator(null));
				return true;
			}
		}
		return IsDestroyed();
	}
}
