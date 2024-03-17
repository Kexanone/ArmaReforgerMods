[BaseContainerProps(configRoot: true)]
class KSC_MissionRotationConfig
{
	[Attribute(desc: "List of scenario IDs for the mission rotations")]
	ref array<ResourceName> m_aScenarioIds;
}