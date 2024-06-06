//------------------------------------------------------------------------------------------------
enum KSC_EJobResult
{
	FAIL,
	SUCCESS,
	RUNNING, // Job is not yet completed and may be continued in this frame
	SUSPEND // Job is not yet completed, but should not be continued in this frame 
}
