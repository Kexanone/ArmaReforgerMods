//------------------------------------------------------------------------------------------------
class KSC_JobSequence : KSC_IJob
{
	protected ref KSC_RefQueue<KSC_IJob> m_qJobs = new KSC_RefQueue<KSC_IJob>();
	
	//------------------------------------------------------------------------------------------------
	void AddJob(KSC_IJob job)
	{
		m_qJobs.Push(job);
	}
	
	//------------------------------------------------------------------------------------------------
	override KSC_EJobResult ExecuteStep(float timeSlice)
	{
		if (m_qJobs.IsEmpty())
			return KSC_EJobResult.SUCCESS;
		
		KSC_EJobResult result = m_qJobs.Front().ExecuteStep(timeSlice);
		if (result == KSC_EJobResult.SUCCESS)
		{
			m_qJobs.Pop();
			
			if (!m_qJobs.IsEmpty())
				result = KSC_EJobResult.RUNNING;
		}
		
		return result;
	}
}
