//------------------------------------------------------------------------------------------------
[BaseContainerProps(configRoot: true)]
class KSC_WorldSlotsConfig
{
	[Attribute()]
	ref array<ref KSC_Location> m_aLocations;
	
	[Attribute()]
	ref KSC_BuildingSlotConfig m_pBuildingSlotConfig;
	
	[Attribute()]
	ref array<ref KSC_TerrainSlotConfig> m_mTerrainSlots;
}
