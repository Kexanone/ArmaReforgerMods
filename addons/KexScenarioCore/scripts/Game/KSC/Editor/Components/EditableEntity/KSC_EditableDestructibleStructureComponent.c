//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class KSC_EditableDestructibleStructureComponentClass : SCR_EditableEntityComponentClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_EditableDestructibleStructureComponent : SCR_EditableEntityComponent
{
	private SCR_DamageManagerComponent m_DamageManager;
	
	//------------------------------------------------------------------------------------------------
	override bool CanDestroy()
	{
		SCR_DamageManagerComponent damageManager = GetDamageManager();
		if (!damageManager)
			return false;
		
		return damageManager.IsDamageHandlingEnabled();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsDestroyed()
	{
		SCR_DamageManagerComponent damageManager = GetDamageManager();
		if (!damageManager)
			return false;
		
		return damageManager.GetState() == EDamageState.DESTROYED;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool Destroy(int editorPlayerID = 0)
	{
		SCR_DamageManagerComponent damageManager = GetDamageManager();
		if (!damageManager)
			return false;
		
		if (!IsServer())
			return false;
		
		if (IsDestroyed() || !CanDestroy())
			return false;
		
		Instigator instigator = Instigator.CreateInstigatorGM(editorPlayerID);
		damageManager.SetAndReplicateInstigator(instigator);
		damageManager.Kill(instigator);
		return IsDestroyed();
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_DamageManagerComponent GetDamageManager()
	{
		if (!m_DamageManager && m_Owner)
		{
			m_DamageManager = SCR_DamageManagerComponent.Cast(m_Owner.FindComponent(SCR_DamageManagerComponent));
			if (!m_DamageManager)
			{
				IEntity child = m_Owner.GetChildren();
				while (child)
				{
					m_DamageManager = SCR_DamageManagerComponent.Cast(child.FindComponent(SCR_DamageManagerComponent));
					if (m_DamageManager)
						break;
					
					child = child.GetSibling();
				}
			}
		}
		
		return m_DamageManager;
	}
}
