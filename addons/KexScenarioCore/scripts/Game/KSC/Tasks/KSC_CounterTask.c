/*****
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
		m_pSupportEntity.SetFormatParams(this, m_iCounter.ToString(), m_iTargetCount.ToString());
		
		if (m_iCounter >= m_iTargetCount)
			m_pSupportEntity.FinishTask(this);
		else
			m_pSupportEntity.PopUpNotification(this, TASK_PROGRESS_TEXT);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTargetCount(int targetCount)
	{
		m_iTargetCount = targetCount;
		m_pSupportEntity.SetFormatParams(this, m_iCounter.ToString(), m_iTargetCount.ToString());
	}
}
*****/