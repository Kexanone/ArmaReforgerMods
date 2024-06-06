//------------------------------------------------------------------------------------------------
class KSC_Job_IWaitUntil : KSC_IJob
{
	//------------------------------------------------------------------------------------------------
	bool Condition() { return true; };
	
	//------------------------------------------------------------------------------------------------
	override KSC_EJobResult ExecuteStep(float timeSlice)
	{
		if (!Condition())
			return KSC_EJobResult.SUSPEND;
		
		return KSC_EJobResult.SUCCESS;
	}
}
