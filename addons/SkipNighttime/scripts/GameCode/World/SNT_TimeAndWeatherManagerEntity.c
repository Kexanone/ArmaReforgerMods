modded class TimeAndWeatherManagerEntity : BaseTimeAndWeatherManagerEntity
{
	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Enables skip nighttime handler", category: "Skip Nighttime")]
	protected bool m_bSNT_enabled;
	[Attribute(defvalue: "120", uiwidget: UIWidgets.EditBox, desc: "Timeout for the daytime checker in seconds. This value gets scaled by the time multiplier (= TimeAndWeatherManagerEntity.GetDayDuration() divided by 86400)", category: "Skip Nighttime")]
	protected float m_fSNT_Timeout;
	// This component currently gets executed before SCR_TimeAndWeatherHandlerComponent, hence it cannot get the correct time multiplier if the GameMode class has a SCR_TimeAndWeatherHandlerComponent
	// Workaround: Set the initial time multiplier to the maximum possible value and the timeout is not changed until EOnInit is done
	protected float m_fSNT_TimeMultiplier = 12;
	protected bool m_bSNT_InitDone = false;
	
	void TimeAndWeatherManagerEntity(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	};
	
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!m_bSNT_enabled)
			return;
		
		// Run handler only on authority
		RplComponent rpl = RplComponent.Cast(owner.FindComponent(RplComponent));
		if (!rpl || rpl.IsProxy())
			return;

		SNT_Handler();
		// Recall handler based on timeout scaled by the time multiplier
		GetGame().GetCallqueue().CallLater(SNT_Handler, m_fSNT_Timeout / m_fSNT_TimeMultiplier * 1000, true);
		m_bSNT_InitDone = true;
	};
	
	void SNT_Handler()
	{
		array<SCR_DayTimeInfoBase> daytimeInfo = new array<SCR_DayTimeInfoBase>;
		GetDayTimeInfoArray(daytimeInfo, GetYear(), GetMonth(), GetDay());
		float sunriseHours24 = daytimeInfo[0].GetDayTime();
		// Sunset hour is adjusted by the timeout such that the skipping happens shortly before the sunset
		float sunsetHours24 = daytimeInfo[3].GetDayTime() - m_fSNT_Timeout / 3600;
		float currHours24 = GetTime().ToTimeOfTheDay();

		if (currHours24 < sunriseHours24 || currHours24 >= sunsetHours24)
		{	
			if (currHours24 >= sunsetHours24)
			{
				// Get the sunrise for the next day
				SNT_DateContainer newDate = SNT_GetNextDay(GetYear(), GetMonth(), GetDay());
				SetDate(newDate.m_iYears, newDate.m_iMonths, newDate.m_iDays);
				GetDayTimeInfoArray(daytimeInfo, newDate.m_iYears, newDate.m_iMonths, newDate.m_iDays);
				sunriseHours24 = daytimeInfo[0].GetDayTime();
			};
			
			SetTime(TimeContainer.FromTimeOfTheDay(sunriseHours24));
		};
		
		// Update timeout if time multiplier has changed
		float currTimeMultiplier = 86400 / GetDayDuration();
		if (m_bSNT_InitDone && (currTimeMultiplier != m_fSNT_TimeMultiplier))
		{
			m_fSNT_TimeMultiplier = currTimeMultiplier;
			GetGame().GetCallqueue().Remove(SNT_Handler);
			GetGame().GetCallqueue().CallLater(SNT_Handler, m_fSNT_Timeout / m_fSNT_TimeMultiplier * 1000, true);
		};
	};

	SNT_DateContainer SNT_GetNextDay(int year, int month, int day)
	{
		day += 1;
		
		if (!CheckValidDate(year, month, day))
		{
			if (month != 12)
			{
				month += 1;
			}	
			else
			{
				month = 1;
				year += 1;
			}
			day = 1;
		};
		
		return SNT_DateContainer(year, month, day);
	};
};

class SNT_DateContainer
{
	int m_iYears;
	int m_iMonths;
	int m_iDays;
	
	void SNT_DateContainer(int years = 0, int months = 0, int days = 0)
	{
		m_iYears = years;
		m_iMonths = months;
		m_iDays = days;
	}
};
