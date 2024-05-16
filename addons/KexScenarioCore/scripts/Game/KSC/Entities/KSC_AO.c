//------------------------------------------------------------------------------------------------
class KSC_AOClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_AO : GenericEntity
{
	/*
	[Attribute()]
	protected ref KSC_AOParams m_Params;
	
	//------------------------------------------------------------------------------------------------
	void Generate()
	{
		
	}
	
	//------------------------------------------------------------------------------------------------
	void SetParams(KSC_AOParams params)
	{
		m_Params = params;
	}
	
	//------------------------------------------------------------------------------------------------
	KSC_AOParams GetParams()
	{
		return m_Params;
	}
	*/
}

//------------------------------------------------------------------------------------------------
class KSC_AOParams : ScriptAndConfig
{
	[Attribute(defvalue: "50", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EAISkill), desc: "Enemy AI Skill Level")]
	protected EAISkill m_eEnemyAISkill;
	
	[Attribute(defvalue: "30", desc: "Minimal enemy AI count")]
	int m_iMinEnemyAICount;
	
	[Attribute(defvalue: "2.5", desc: "Total enemy AI count will be this multiplier times the total player count (ignored when below m_iMinEnemyAICount)")]
	float m_fEnemyAICountMultiplier;
	
	[Attribute(defvalue: "true", desc: "Reinforcements are automatically spawned if AI count is below the target count")]
	bool m_bEnableReinforements;
}
