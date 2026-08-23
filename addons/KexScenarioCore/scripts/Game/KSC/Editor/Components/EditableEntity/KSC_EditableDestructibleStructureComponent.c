//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class KSC_EditableDestructibleStructureComponentClass : SCR_EditableEntityComponentClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_EditableDestructibleStructureComponent : SCR_EditableEntityComponent
{
	protected SCR_DamageManagerComponent m_DamageManager;
	
	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
				
		m_DamageManager = SCR_DamageManagerComponent.Cast(owner.FindComponent(SCR_DamageManagerComponent));
		if (!m_DamageManager)
		{
			IEntity child = owner.GetChildren();
			while (child)
			{
				m_DamageManager = SCR_DamageManagerComponent.Cast(child.FindComponent(SCR_DamageManagerComponent));
				if (m_DamageManager)
					break;
				
				child = child.GetSibling();
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanDestroy()
	{
		if (!m_DamageManager)
			return false;
		
		return m_DamageManager.IsDamageHandlingEnabled();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsDestroyed()
	{
		if (!m_DamageManager)
			return false;
		
		return m_DamageManager.GetState() == EDamageState.DESTROYED;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool Destroy(int editorPlayerID = 0)
	{
		if (!m_DamageManager)
			return false;
		
		if (!IsServer())
			return false;
		
		if (IsDestroyed() || !CanDestroy())
			return false;
		
		Instigator instigator = Instigator.CreateInstigatorGM(editorPlayerID);
		m_DamageManager.SetAndReplicateInstigator(instigator);
		m_DamageManager.Kill(instigator);
		return IsDestroyed();
	}
	
	//------------------------------------------------------------------------------------------------
	DamageManagerComponent GetDamageManager()
	{
		return m_DamageManager;
	}
}
