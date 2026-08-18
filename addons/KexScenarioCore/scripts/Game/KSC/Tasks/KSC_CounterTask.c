//------------------------------------------------------------------------------------------------
class KSC_CounterTaskClass : KSC_BaseTaskClass
{
}

//------------------------------------------------------------------------------------------------
//! Implements pop-up notifications, script invokers, title and description formatting.
class KSC_CounterTask : KSC_BaseTask
{
	[Attribute(defvalue: "3", uiwidget: UIWidgets.Slider, params: "1 inf", desc: "Target count for the task to complete")]
	protected int m_iTargetCount;
	protected int m_iCounter = 0;
	
	//------------------------------------------------------------------------------------------------
	void IncrementCounter()
	{
		m_iCounter++;
		bool isCompleted = (m_iCounter >= m_iTargetCount);
		UpdateParams({m_iCounter.ToString(), m_iTargetCount.ToString()}, hasTaskProgressed: !isCompleted);
		
		if (isCompleted)
			s_pTaskSystem.SetTaskState(this, SCR_ETaskState.COMPLETED);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetParams(Faction targetFaction, int count = 0, int targetCount = -1, array<LocalizedString> formatParams = null)
	{
		array<LocalizedString> extendedFormatParams = {};
		if (formatParams)
			extendedFormatParams = formatParams;
		
		m_iCounter = count;
		
		if (targetCount >= 0)
			m_iTargetCount = targetCount;
		
		extendedFormatParams.InsertAt(m_iTargetCount.ToString(), 0);
		extendedFormatParams.InsertAt(m_iCounter.ToString(), 0);
		super.SetParams(targetFaction, extendedFormatParams);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTargetCount(int targetCount)
	{
		m_iTargetCount = targetCount;
		UpdateParams({m_iCounter.ToString(), m_iTargetCount.ToString()});
	}
}
