//------------------------------------------------------------------------------------------------
modded class SCR_PlayerListEntry
{
	TextWidget m_wSoftKills;
	TextWidget m_wArmorKills;
	TextWidget m_wAirKills;
}

//------------------------------------------------------------------------------------------------
modded class SCR_PlayerListMenu : SCR_SuperMenuBase
{
	//------------------------------------------------------------------------------------------------
	override protected void InitSorting()
	{
		super.InitSorting();
		
		if (m_ScoringSystem)
			return;
		
		if (!GetRootWidget())
			return;

		Widget w = GetRootWidget().FindAnyWidget("SortHeader");
		if (!w)
			return;

		// Hide K/D/S sorting headers if the re is no scoreboard
		ButtonWidget sortSoftKills = ButtonWidget.Cast(w.FindAnyWidget("KST_sortSoftKills"));
		ButtonWidget sortArmorKills = ButtonWidget.Cast(w.FindAnyWidget("KST_sortArmorKills"));
		ButtonWidget sortAirKills = ButtonWidget.Cast(w.FindAnyWidget("KST_sortAirKills"));

		if (sortSoftKills)
			sortSoftKills.SetOpacity(0);
		if (sortArmorKills)
			sortArmorKills.SetOpacity(0);
		if (sortAirKills)
			sortAirKills.SetOpacity(0);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void Sort(string filterName, bool sortUp)
	{
		if (!m_ScoringSystem)
			super.Sort(filterName, sortUp);
		else
			SortByScore(true);
	}
	
	//------------------------------------------------------------------------------------------------
	override void CreateEntry(int id, SCR_PlayerDelegateEditorComponent editorDelegateManager)
	{
		super.CreateEntry(id, editorDelegateManager);
		SCR_PlayerListEntry currentEntry;
		
		foreach (SCR_PlayerListEntry entry : m_aEntries)
		{
			if (entry.m_iID == id)
			{
				currentEntry = entry;
				break;
			}
		}
		
		if (!currentEntry || !currentEntry.m_wRow)
			return;

		currentEntry.m_wSoftKills = TextWidget.Cast(currentEntry.m_wRow.FindAnyWidget("KST_SoftKills"));
		currentEntry.m_wArmorKills = TextWidget.Cast(currentEntry.m_wRow.FindAnyWidget("KST_ArmorKills"));
		currentEntry.m_wAirKills = TextWidget.Cast(currentEntry.m_wRow.FindAnyWidget("KST_AirKills"));
		if (currentEntry.m_Info)
		{
			if (currentEntry.m_wSoftKills)
				currentEntry.m_wSoftKills.SetText(currentEntry.m_Info.m_iKST_SoftKills.ToString());
			if (currentEntry.m_wArmorKills)
				currentEntry.m_wArmorKills.SetText(currentEntry.m_Info.m_iKST_ArmorKills.ToString());
			if (currentEntry.m_wAirKills)
				currentEntry.m_wAirKills.SetText(currentEntry.m_Info.m_iKST_AirKills.ToString());
		}
		else
		{
			if (currentEntry.m_wSoftKills)
				currentEntry.m_wSoftKills.SetText("");
			if (currentEntry.m_wArmorKills)
				currentEntry.m_wArmorKills.SetText("");
			if (currentEntry.m_wAirKills)
				currentEntry.m_wAirKills.SetText("");
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void UpdateScore()
	{
		if (!m_aAllPlayersInfo || m_aAllPlayersInfo.Count() == 0 || !m_ScoringSystem)
			return;
		
		foreach (SCR_PlayerListEntry entry : m_aEntries)
		{
			foreach (int playerId, SCR_ScoreInfo info : m_aAllPlayersInfo)
			{
				if (!info || playerId != entry.m_iID)
					continue;

				entry.m_Info = info;
				break;
			}

			if (!entry.m_Info)
				continue;

			if (entry.m_wSoftKills)
				entry.m_wSoftKills.SetText(entry.m_Info.m_iKST_SoftKills.ToString());
			if (entry.m_wArmorKills)
				entry.m_wArmorKills.SetText(entry.m_Info.m_iKST_ArmorKills.ToString());
			if (entry.m_wAirKills)
				entry.m_wAirKills.SetText(entry.m_Info.m_iKST_AirKills.ToString());
		}
		
		super.UpdateScore();
	}
}