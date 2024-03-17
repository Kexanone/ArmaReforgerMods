class KSC_PrefabCatalogue_BaseConfig : Managed
{
}

class KSC_PrefabCatalogue_EntityConfig : KSC_PrefabCatalogue_BaseConfig
{
	ref array<ref EEditableEntityLabel>> m_aLabels;
}

class KSC_PrefabCatalogue_GroupConfig : KSC_PrefabCatalogue_BaseConfig
{
	ref array<ref KSC_PrefabCatalogue_EntityConfig> m_aMemberConfigs;
	string m_sFormationName = "Wedge";
}

//------------------------------------------------------------------------------------------------
class KSC_PrefabCatalogueComponentClass : SCR_BaseGameModeComponentClass
{
}

//------------------------------------------------------------------------------------------------
//! Compoenent for compiling prefab catalogues
class KSC_PrefabCatalogueComponent : SCR_BaseGameModeComponent
{
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
	}
	
	SCR_ChimeraCharacter SpawnCharacter(KSC_PrefabCatalogue_EntityConfig config, vector pos = vector.Zero, float rotation = 0)
	{
		
	}
	
	AIGroup SpawnGroup(KSC_PrefabCatalogue_GroupConfig config, vector pos = vector.Zero, float rotation = 0)
	{
		
	}
	
	Vehicle SpawnVehicle(KSC_PrefabCatalogue_EntityConfig config, vector pos = vector.Zero, float rotation = 0)
	{
		
	}
	
	IEntity SpawnStructure(KSC_PrefabCatalogue_EntityConfig config, vector pos = vector.Zero, float rotation = 0)
	{
		
	}
}