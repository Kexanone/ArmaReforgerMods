//------------------------------------------------------------------------------------------------
class KSC_EditableDestructibleStructureComponentSerializer : SCR_EditableEntityComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return KSC_EditableDestructibleStructureComponent;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{
		ESerializeResult result = super.Serialize(owner, component, context);
		if (result == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
		
		const KSC_EditableDestructibleStructureComponent editable = KSC_EditableDestructibleStructureComponent.Cast(component);
		if (!editable.IsDestroyed())
			return ESerializeResult.DEFAULT;
		
		context.WriteValue("ksc_version", 1);
		context.WriteValue("ksc_destroyed", true);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		if (!super.Deserialize(owner, component, context))
			return false;
		
		KSC_EditableDestructibleStructureComponent editable = KSC_EditableDestructibleStructureComponent.Cast(component);
		
		int version;
		context.ReadValue("ksc_version", version);
		
		bool isDestroyed;
		if (context.ReadValue("ksc_destroyed", isDestroyed) && isDestroyed)
			editable.Destroy();
		
		return true;
	}
}
