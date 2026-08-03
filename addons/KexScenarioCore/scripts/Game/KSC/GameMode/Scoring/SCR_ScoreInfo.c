//------------------------------------------------------------------------------------------------
modded class SCR_ScoreInfo
{
	int m_iKSC_SoftKills;
	int m_iKSC_ArmorKills;
	int m_iKSC_AirKills;
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
    {
		super.RplSave(writer);
        writer.WriteInt(m_iKSC_SoftKills);
		writer.WriteInt(m_iKSC_ArmorKills);
		writer.WriteInt(m_iKSC_AirKills);
		return true;
    }

	//------------------------------------------------------------------------------------------------
    override bool RplLoad(ScriptBitReader reader)
    {
		super.RplLoad(reader);
		reader.ReadInt(m_iKSC_SoftKills);
		reader.ReadInt(m_iKSC_ArmorKills);
		reader.ReadInt(m_iKSC_AirKills);
        return true;
    }
	
	//------------------------------------------------------------------------------------------------
	override void Clear()
	{
		super.Clear();
		m_iKSC_SoftKills = 0;
		m_iKSC_ArmorKills = 0;
		m_iKSC_AirKills = 0;
	}
};
