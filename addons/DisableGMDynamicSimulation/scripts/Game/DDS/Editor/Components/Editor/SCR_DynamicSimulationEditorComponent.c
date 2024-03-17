//------------------------------------------------------------------------------------------------
modded class SCR_DynamicSimulationEditorComponent : SCR_BaseEditorComponent
{
	//------------------------------------------------------------------------------------------------
	//! Only stream players
	override protected void EnableStreamingForEntity(SCR_EditableEntityComponent entity, bool enable)
	{
		if (!EntityUtils.IsPlayer(entity.GetOwner()))
			return;
		
		super.EnableStreamingForEntity(entity, enable);
	}
}