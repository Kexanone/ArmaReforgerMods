//------------------------------------------------------------------------------------------------
class KSC_Job_Wait : KSC_IJob
{
	protected float m_fDelay;
	protected float m_fTimer = 0;
	
	//------------------------------------------------------------------------------------------------
	void KSC_Job_Wait(float delay)
	{
		m_fDelay = delay;
	}
	
	//------------------------------------------------------------------------------------------------
	override KSC_EJobResult ExecuteStep(float timeSlice)
	{
		m_fTimer += timeSlice;
		
		if (m_fTimer < m_fDelay)
			return KSC_EJobResult.SUSPEND;
		
		return KSC_EJobResult.SUCCESS;
	}
}
