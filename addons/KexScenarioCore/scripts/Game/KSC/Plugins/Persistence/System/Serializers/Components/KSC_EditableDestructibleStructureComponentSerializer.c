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
		const KSC_EditableDestructibleStructureComponent editable = KSC_EditableDestructibleStructureComponent.Cast(component);
		
		context.StartObject("base");
		const ESerializeResult baseResult = super.Serialize(owner, component, context);
		context.EndObject();
		
		if (baseResult == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
		
		const bool destroyed = editable.IsDestroyed();
		
		if (baseResult == ESerializeResult.DEFAULT &&
			!destroyed)
		{
			return ESerializeResult.DEFAULT;
		}
		
		context.WriteValue("version", 1);
		context.WriteDefault(destroyed, false);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		KSC_EditableDestructibleStructureComponent editable = KSC_EditableDestructibleStructureComponent.Cast(component);
		
		if (context.DoesObjectExist("base"))
		{
			if (!context.StartObject("base") ||
				!super.Deserialize(owner, component, context) ||
				!context.EndObject())
			{
				return false;
			}
		}
		
		int version;
		context.Read(version);
		
		bool destroyed;
		if (context.Read(destroyed) && destroyed)
			editable.Destroy();
		
		return true;
	}
}
