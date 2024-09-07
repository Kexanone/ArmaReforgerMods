//------------------------------------------------------------------------------------------------
class KSC_SpawnGroupHelperComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
//! Helper component on AIGroup for KSC_GameTools.SpawnGroupPrefab
//! Allows spawning members at correct altitute and in a compact manner
//! Also AI skill can be changed
class KSC_SpawnGroupHelperComponent : ScriptComponent
{
	protected EAISkill m_eSkill = EAISkill.REGULAR;
	protected SCR_AIGroup m_pGroup;
	protected vector m_vCenter;
	protected const float OFFSET_STEPSIZE = 0.75;
	float m_fX = 0, m_fZ = 0;
	float m_fDX = 0, m_fDZ = -OFFSET_STEPSIZE;

	//------------------------------------------------------------------------------------------------
	void Enable(bool enable)
	{
		m_pGroup = SCR_AIGroup.Cast(GetOwner());
		m_pGroup.GetOnAgentAdded().Remove(OnAgentAdded);
		
		if (enable)
			m_pGroup.GetOnAgentAdded().Insert(OnAgentAdded);
	}
	
	//------------------------------------------------------------------------------------------------
	// Sets AI skill and arranges them in a compact spiral
	void OnAgentAdded(AIAgent agent)
	{
		IEntity entity = agent.GetControlledEntity();
		if (!entity)
			return;
		
		if (m_pGroup.GetAgentsCount() > 0)
		{
			if (m_fX == m_fZ || (m_fX < 0 && m_fX == -m_fZ) || (m_fX > 0 || m_fX == OFFSET_STEPSIZE - m_fZ))
			{
				m_fDX = -m_fDZ;
				m_fDZ = m_fDX;
			}
			
			m_fX += m_fDX;
			m_fZ += m_fDZ;
		}
		
		entity.SetOrigin(m_vCenter + Vector(m_fX, 0, m_fZ));
		
		// Set AI Skill
		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(entity.FindComponent(SCR_AICombatComponent));
		combatComponent.SetAISkill(m_eSkill);
		
		// Disable once all members have been added
		if (m_pGroup.GetNumberOfMembersToSpawn() == m_pGroup.GetAgentsCount())
			Enable(false);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSkill(EAISkill skill)
	{
		m_eSkill = skill;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCenter(vector center)
	{
		m_vCenter = center;
	}
}