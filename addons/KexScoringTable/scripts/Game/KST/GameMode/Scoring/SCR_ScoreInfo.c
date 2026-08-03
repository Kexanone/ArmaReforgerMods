//------------------------------------------------------------------------------------------------
modded class SCR_ScoreInfo
{
	int m_iKST_SoftKills;
	int m_iKST_ArmorKills;
	int m_iKST_AirKills;
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
    {
		super.RplSave(writer);
        writer.WriteInt(m_iKST_SoftKills);
		writer.WriteInt(m_iKST_ArmorKills);
		writer.WriteInt(m_iKST_AirKills);
		return true;
    }

	//------------------------------------------------------------------------------------------------
    override bool RplLoad(ScriptBitReader reader)
    {
		super.RplLoad(reader);
		reader.ReadInt(m_iKST_SoftKills);
		reader.ReadInt(m_iKST_ArmorKills);
		reader.ReadInt(m_iKST_AirKills);
        return true;
    }
	
	//------------------------------------------------------------------------------------------------
	override void Clear()
	{
		super.Clear();
		m_iKST_SoftKills = 0;
		m_iKST_ArmorKills = 0;
		m_iKST_AirKills = 0;
	}
};
