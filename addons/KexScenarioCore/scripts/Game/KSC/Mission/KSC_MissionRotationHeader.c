//------------------------------------------------------------------------------------------------
class KSC_MissionRotationHeader : SCR_MissionHeader
{
	[Attribute(defvalue: "false", desc: "If true, first mission of the rotation is selected randomly")]
	bool m_bSelectRandomFirstMission;
}
