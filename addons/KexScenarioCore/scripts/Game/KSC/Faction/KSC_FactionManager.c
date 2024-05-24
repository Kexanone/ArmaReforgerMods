//------------------------------------------------------------------------------------------------
class KSC_FactionManagerClass: SCR_FactionManagerClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_FactionManager : SCR_FactionManager
{
	protected ref array<string> m_aVanillaFactionKeys = {"US", "USSR", "FIA"};
	protected ref map<Faction, Faction> m_mVanillaFactionMap = new map<Faction, Faction>();
	
	//------------------------------------------------------------------------------------------------
	bool IsVanillaFaction(Faction faction)
	{
		return m_aVanillaFactionKeys.Contains(faction.GetFactionKey());
	}
		
	//------------------------------------------------------------------------------------------------
	//! Used for mod factions like USMC from RHS to get the the associated vanilla faction (US)
	//! The vanilla faction is currently determined via the radio encryption key
	Faction GetAssociatedVanillaFaction(Faction faction)
	{
		Faction vanillaFaction;
		if (m_mVanillaFactionMap.Find(faction, vanillaFaction))
			return vanillaFaction;
		
		SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
		if (!scriptedFaction)
			return null;
		
		string encryptionKey = scriptedFaction.GetFactionRadioEncryptionKey();
		
		foreach (string key : m_aVanillaFactionKeys)
		{
			SCR_Faction scriptedVanillaFaction = SCR_Faction.Cast(GetFactionByKey(key));
			if (!scriptedVanillaFaction)
				continue;
						
			if (scriptedVanillaFaction.GetFactionRadioEncryptionKey() != encryptionKey)
				continue;
			
			m_mVanillaFactionMap[scriptedFaction] = scriptedVanillaFaction;
			return scriptedVanillaFaction;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	void GetFactionEntityListWithLabel(Faction faction, EEntityCatalogType catalogType, EEditableEntityLabel label, out notnull array<ResourceName> entityNames)
	{
		GetFactionEntityListWithLabels(faction, catalogType, {label}, entityNames);
	}
	
	//------------------------------------------------------------------------------------------------
	void GetFactionEntityListWithLabels(Faction faction, EEntityCatalogType catalogType, notnull array<EEditableEntityLabel> labels, out notnull array<ResourceName> entityNames)
	{
		SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
		if (!scriptedFaction)
			return;
		
		array<SCR_EntityCatalogEntry> entries = {};
		
		SCR_EntityCatalog catalog = scriptedFaction.GetFactionEntityCatalogOfType(catalogType);
		if (catalog)
			catalog.GetFullFilteredEntityListWithLabels(entries, labels);
		
		if (!entries.IsEmpty())
		{
			entityNames.Reserve(entityNames.Count() + entries.Count());
			foreach (SCR_EntityCatalogEntry entry : entries)
			{
				ResourceName prefabName = entry.GetPrefab();
				
				if (prefabName.IndexOf("NotSpawned") >= 0)
					continue;
				
				entityNames.Insert(prefabName);
			}
		}
		else
		{
			if (!IsVanillaFaction(faction))
				GetFactionEntityListWithLabels(GetAssociatedVanillaFaction(faction), catalogType, labels, entityNames);
		}	
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_EScenarioFrameworkMarkerCustomColor GetMarkerCustomColorEntry(Faction faction)
	{
		string factionKey;
		faction = GetAssociatedVanillaFaction(faction);
		if (faction)
			factionKey = faction.GetFactionKey();
		
		switch (factionKey)
		{
			case "US":
			{
				return SCR_EScenarioFrameworkMarkerCustomColor.BLUFOR;
			}
			
			case "USSR":
			{
				return SCR_EScenarioFrameworkMarkerCustomColor.OPFOR;
			}
			
			case "FIA":
			{
				return SCR_EScenarioFrameworkMarkerCustomColor.INDEPENDENT;
			}
		}
		
		return SCR_EScenarioFrameworkMarkerCustomColor.REFORGER_ORANGE;
	}
}
