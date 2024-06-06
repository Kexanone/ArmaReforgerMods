//------------------------------------------------------------------------------------------------
//! Simple job system for running scripts in a scheduled environment
//! Schedule your jobs via KSC_JobSystem.Submit
class KSC_JobSystem : GameSystem
{
	static private KSC_JobSystem m_Instance; 
	
	protected ref array<ref KSC_IJob> m_aJobs = {};
	// Keeps track which job is suspended for current frame
	protected ref array<bool> m_aJobSuspendedForFrame = {};
	protected int m_iCurrentJobIdx = -1;
	// Keeps track how many jobs are left for the current frame
	// Gets decremented when a job is completed or suspended
	protected int m_iNumJobsLeftForFrame = 0;
	
	// Scheduled environment in SQF can run up to 3 ms
	[Attribute(defvalue: "3", desc: "Maximum mean runtime per frame in ms that the system can run")]
	protected int m_iMaxMeanTicksPerFrame;
	protected int m_iStartTick = 0;
	protected int m_iOverticks = 0;
	
	//------------------------------------------------------------------------------------------------
	static KSC_JobSystem GetInstance()
	{
		if (!m_Instance)
		{
			World world = GetGame().GetWorld();
			m_Instance = KSC_JobSystem.Cast(world.FindSystem(KSC_JobSystem));
		}
		
		return m_Instance;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Submits a job to be scheduled for execution
	static void Submit(KSC_IJob job)
	{
		KSC_JobSystem instance = GetInstance();
		
		if (!instance.IsEnabled())
			instance.Enable(true);
		
		instance.m_aJobs.Insert(job);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Turn off if no jobs are scheduled
	override protected void OnInit()
	{
		if (m_aJobs.IsEmpty())
			Enable(false);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Manages job scheduling and execution
	override protected void OnUpdate(ESystemPoint point)
	{
		float timeSlice = GetWorld().GetTimeSlice();
		m_iStartTick = System.GetTickCount();
		
		// Compute how many ticks we have available
		int currentMaxTicks = m_iMaxMeanTicksPerFrame;
		if (m_iOverticks > 0)
		{
			currentMaxTicks -= m_iOverticks;
			m_iOverticks -= m_iMaxMeanTicksPerFrame;
		}
		
		if (currentMaxTicks <= 0)
			return;
		
		// Reset job states for frame
		m_iNumJobsLeftForFrame = m_aJobs.Count();
		m_aJobSuspendedForFrame.Resize(m_iNumJobsLeftForFrame);
		
		for (int i = 0; i < m_iNumJobsLeftForFrame; i++)
		{
			m_aJobSuspendedForFrame[i] = false;
		}
		
		// Execute steps while we have jobs and ticks left
		while (m_iNumJobsLeftForFrame > 0 && (System.GetTickCount(m_iStartTick) - currentMaxTicks < 0))
		{
			// Pick and process next job step
			m_iCurrentJobIdx = Math.Mod(m_iCurrentJobIdx + 1, m_aJobs.Count());
			
			if (m_aJobSuspendedForFrame[m_iCurrentJobIdx])
				continue;
			
			KSC_EJobResult result = m_aJobs[m_iCurrentJobIdx].ExecuteStep(timeSlice);
			
			switch (result)
			{
				// Remove job if completed
				case KSC_EJobResult.SUCCESS:
				case KSC_EJobResult.FAIL:
				{
					m_aJobs.Remove(m_iCurrentJobIdx);
					m_iNumJobsLeftForFrame--;
					break;
				}
				
				// Mark job as suspended
				case KSC_EJobResult.SUSPEND:
				{
					m_aJobSuspendedForFrame[m_iCurrentJobIdx] = true;
					m_iNumJobsLeftForFrame--;
					break;
				}
			}
		}
				
		// Turn off system if all jobs are done
		if (m_aJobs.IsEmpty())
			Enable(false);
		
		// Compute how many ticks we lose in the next frames
		m_iOverticks = System.GetTickCount(m_iStartTick) - currentMaxTicks;
	}
}

//------------------------------------------------------------------------------------------------
class HelloWorldJob : KSC_IJob
{
	//------------------------------------------------------------------------------------------------
	override KSC_EJobResult ExecuteStep(float timeSlice)
	{
		Print("Hello World!");
		return KSC_EJobResult.SUCCESS;
	}
}
