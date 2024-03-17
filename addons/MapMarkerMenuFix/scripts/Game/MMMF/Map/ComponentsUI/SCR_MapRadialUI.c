//------------------------------------------------------------------------------------------------
//! 2d map radial menu UI
modded class SCR_MapRadialUI: SCR_MapUIBaseComponent
{
	//------------------------------------------------------------------------------------------------
	//! SCR_RadialMenuController event
	override protected void InputOpenMenu(SCR_RadialMenuController controller, bool hasControl)
	{
		float x, y, w, h;
		GetGame().GetWorkspace().GetScreenPos(x, y);
		GetGame().GetWorkspace().GetScreenSize(w, h);
		m_RadialMenu.SetMenuCenterPos(Vector(x, y, 0) + Vector(w, h, 0) / 2);
		super.InputOpenMenu(controller, hasControl);
	}	
}