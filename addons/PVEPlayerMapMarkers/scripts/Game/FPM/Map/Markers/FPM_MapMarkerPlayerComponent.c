//------------------------------------------------------------------------------------------------
class FPM_MapMarkerPlayerComponent : SCR_MapMarkerDynamicWComponent
{
	protected bool m_bIsSymbolMode;
	protected Widget m_wSymbolRoot;
	protected Widget m_wSymbolOverlay;
	protected bool m_bIsHovered = false;
	
	//------------------------------------------------------------------------------------------------
	//! Set visual mode for military symbol which is constructed through additional component
	void SetMilitarySymbolMode(bool state)
	{
		m_bIsSymbolMode = state;
		
		m_wSymbolRoot.SetEnabled(state);
		m_wSymbolRoot.SetVisible(state);
		
		m_wMarkerIcon.SetVisible(!state);
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateMilitarySymbol(SCR_MilitarySymbol milSymbol)
	{
		SCR_MilitarySymbolUIComponent symbolComp = SCR_MilitarySymbolUIComponent.Cast(m_wSymbolOverlay.FindHandler(SCR_MilitarySymbolUIComponent));
		if (symbolComp)
			symbolComp.Update(milSymbol);
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetColor(Color color)
	{
		super.SetColor(color);
		m_wSymbolOverlay.SetColor(color);
	}
	
	//------------------------------------------------------------------------------------------------
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		m_wSymbolRoot = m_wRoot.FindAnyWidget("SymbolWidget");
		m_wSymbolOverlay = m_wSymbolRoot.FindAnyWidget("SymbolOverlay");
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_bIsHovered = true;
		UpdateTextVisibility();
		return super.OnMouseEnter(w, x, y);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_bIsHovered = false;
		UpdateTextVisibility();
		return super.OnMouseEnter(w, x, y);
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateTextVisibility()
	{
		if (m_iLayerID <= 1)
		{
			SetTextVisible(true);
		}
		else
		{
			SetTextVisible(m_bIsHovered);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0)	// LMB only
			return true;
		
		GetGame().OpenGroupMenu();
		
		return true;
	}
}
