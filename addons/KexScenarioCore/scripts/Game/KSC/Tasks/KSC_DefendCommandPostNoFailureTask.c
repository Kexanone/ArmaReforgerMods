/*****
//------------------------------------------------------------------------------------------------
class KSC_DefendCommandPostNoFailureTaskClass : KSC_SubjectBaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! This task gets completed when the target faction controls a command post (CP) after a certain
//! timeout. This task cannot fail. If the target faction does not control the CP after the
//! timeout, it will be completed once the target faction recaptured the CP.
class KSC_DefendCommandPostNoFailureTask : KSC_SubjectBaseTask
{
	[Attribute(defvalue: "720", desc: "Minimal duration of the defense in seconds")]
	protected float m_fMinDefenseDuration;
	
	[Attribute(defvalue: "60", desc: "Timeout in seconds for faction affiliation checks")]
	protected float m_fFactionCheckTimeout;
	
	protected SCR_FactionAffiliationComponent m_pFactionAffiliation;
	
	//------------------------------------------------------------------------------------------------
	override protected void AttachSubjectHandlers()
	{
		super.AttachSubjectHandlers();
		
		if (!m_pSubject)
			return;
		
		m_pFactionAffiliation = SCR_FactionAffiliationComponent.Cast(m_pSubject.FindComponent(SCR_FactionAffiliationComponent));
		IEntity child = m_pSubject.GetChildren();
		
		while (!m_pFactionAffiliation && child)
		{
			m_pFactionAffiliation = SCR_FactionAffiliationComponent.Cast(child.FindComponent(SCR_FactionAffiliationComponent));			
			child = child.GetSibling();
		};
		
		GetGame().GetCallqueue().CallLater(CheckAffiliatedFaction, m_fMinDefenseDuration*1000, false);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finish task when command post is under control of the target faction
	//! Otherwise schedule a check for later
	void CheckAffiliatedFaction()
	{
		if (!m_pFactionAffiliation || !m_pSupportEntity)
			return;
		
		if (m_pFactionAffiliation.GetAffiliatedFaction() != GetTargetFaction())
		{
			GetGame().GetCallqueue().CallLater(CheckAffiliatedFaction, m_fFactionCheckTimeout*1000, false);
			return;
		};
		
		m_pSupportEntity.FinishTask(this);			
	}
}
*****/