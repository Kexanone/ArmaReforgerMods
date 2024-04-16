//------------------------------------------------------------------------------------------------
[EntityEditorProps(category: "GameScripted/Markers")]
class FPM_MapMarkerPlayerClass : SCR_MapMarkerEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Dynamic map marker -> player
class FPM_MapMarkerPlayer : SCR_MapMarkerEntity
{
	protected static const ref const Color COLOR_INCAPACITATED = Color.FromInt(0xFFFF3636);
	protected static const ref const Color COLOR_DEAD = Color.Gray;
	
	[RplProp(onRplName: "OnUpdateSymbol")]
	protected ref SCR_MilitarySymbol m_Symbol;
	
	[RplProp(onRplName: "OnUpdateColor")]
	protected int m_iColor = Color.WHITE;
	
	[RplProp(onRplName: "OnUpdateText")]
	protected string m_sSyncedText = string.Empty;
	
	protected FPM_MapMarkerPlayerComponent m_pFPM_MarkerWidgetComp;
	
	[RplProp(onRplName: "InitializeColor")]
	protected int m_iColorAlive;
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		m_Symbol = new SCR_MilitarySymbol();
		m_Symbol.SetIdentity(EMilitarySymbolIdentity.BLUFOR);
		m_Symbol.SetDimension(EMilitarySymbolDimension.LAND);
		m_Symbol.SetIcons(EMilitarySymbolIcon.INFANTRY);
	}
	
	//------------------------------------------------------------------------------------------------
	void InitializeColor()
	{
		m_iColor = m_iColorAlive;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnCreateMarker()
	{
		super.OnCreateMarker();
		m_pFPM_MarkerWidgetComp = FPM_MapMarkerPlayerComponent.Cast(m_MarkerWidgetComp);
		if (!m_pFPM_MarkerWidgetComp)
			return;
		
		m_pFPM_MarkerWidgetComp.SetMilitarySymbolMode(true);
		m_pFPM_MarkerWidgetComp.UpdateMilitarySymbol(m_Symbol);
		m_pFPM_MarkerWidgetComp.SetColor(Color.FromInt(m_iColor));
		m_pFPM_MarkerWidgetComp.SetText(m_sSyncedText);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetGlobalSymbolIcons(EMilitarySymbolIcon icons)
	{
		m_Symbol.SetIcons(icons);
		OnUpdateSymbol();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnUpdateSymbol()
	{
		if (m_pFPM_MarkerWidgetComp)	
			m_pFPM_MarkerWidgetComp.UpdateMilitarySymbol(m_Symbol);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetGlobalColor(Color color)
	{
		m_iColor = color.PackToInt();
		OnUpdateColor();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnUpdateColor()
	{
		if (m_pFPM_MarkerWidgetComp)
			m_pFPM_MarkerWidgetComp.SetColor(Color.FromInt(m_iColor));
	}
	
	//------------------------------------------------------------------------------------------------
	void SetGlobalText(string text)
	{
		m_sSyncedText = text;
		OnUpdateText();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnUpdateText()
	{
		if (m_pFPM_MarkerWidgetComp)
			m_pFPM_MarkerWidgetComp.SetText(m_sSyncedText);
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsGlobalVisible()
	{
		return m_bIsGlobalVisible;
	}
	
	//------------------------------------------------------------------------------------------------
	void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		switch (newLifeState)
		{
			case ECharacterLifeState.ALIVE: { SetGlobalColor(Color.FromInt(m_iColorAlive)); return; };
			case ECharacterLifeState.INCAPACITATED: { SetGlobalColor(COLOR_INCAPACITATED); return; };
			case ECharacterLifeState.DEAD: { SetGlobalColor(COLOR_DEAD); return; };
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetFaction(Faction faction)
	{
		super.SetFaction(faction);
		
		SCR_GroupIdentityCore core = SCR_GroupIdentityCore.Cast(SCR_GroupIdentityCore.GetInstance(SCR_GroupIdentityCore));
		if (!core)
			return;
		
		SCR_MilitarySymbolRuleSet ruleSet = core.GetSymbolRuleSet();
		if (!ruleSet)
			return;
		
		ruleSet.UpdateSymbol(m_Symbol, SCR_Faction.Cast(faction));
		OnUpdateSymbol();
		m_iColorAlive = faction.GetFactionColor().PackToInt();
		InitializeColor();
		Replication.BumpMe();
	}
}