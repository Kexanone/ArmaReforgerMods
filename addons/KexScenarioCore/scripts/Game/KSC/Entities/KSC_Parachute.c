//------------------------------------------------------------------------------------------------
class KSC_ParachuteClass : ACE_AnimationHelperCompartmentClass
{
}

//------------------------------------------------------------------------------------------------
class KSC_Parachute : ACE_AnimationHelperCompartment
{
	[Attribute(defvalue: "-4", desc: "Terminal velocity of the chute [m/s]")]
	protected float m_fTerminalVelocity;
	
	[Attribute(defvalue: "0.5", desc: "Rate constant for convergence of horizontal velocity towards wind velocity [1/s]")]
	protected float m_fHorizontalRateConstant;
	
	[Attribute(defvalue: "2.2", desc: "Rate constant for convergence of vertical velocity towards terminal chute velocity [1/s]")]
	protected float m_fVerticalRateConstant;
	
	[RplProp(onRplName: "OnToggleFrame")]
	protected bool m_bIsFrameToggledOn = false;
	
	[RplProp()]
	protected float m_fMorphRate = -1/3; // [1/s] Default value is morph rate for opening
	protected float m_fClosingMorphRate = 1; // [1/s]
	
	[RplProp()]
	protected float m_fMorphProgress = 1;
	
	protected ChimeraWorld m_pWorld;
	protected BaseWeatherManagerEntity m_pWeatherManager;
	protected ref TraceParam m_pTraceParam = new TraceParam();
	
	//------------------------------------------------------------------------------------------------
	override void Init(SCR_ChimeraCharacter performer)
	{
		super.Init(performer);
		m_pWorld = GetGame().GetWorld();
		m_pWeatherManager = m_pWorld.GetTimeAndWeatherManager();
		GetPhysics().ChangeSimulationState(SimulationState.SIMULATION);
		GetPhysics().SetVelocity(performer.GetPhysics().GetVelocity());
		m_pTraceParam.Flags = TraceFlags.ENTS | TraceFlags.OCEAN | TraceFlags.WORLD;
		m_pTraceParam.Exclude = performer;
		SetEventMask(EntityEvent.SIMULATE | EntityEvent.FIXEDFRAME);
		ToggleFrame(true);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Do not terminate when performer gets incapacitated or dies
	override protected void OnPerformerLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	//! Propagation of velocity on server (sync in MP is done via NwkPhysicsMovementComponent)
	//! A simple exponential decay is used for converging the velocity towards a target velocity
	override protected void EOnSimulate(IEntity owner, float timeSlice)
	{
		super.EOnSimulate(owner, timeSlice);
		vector v = GetPhysics().GetVelocity();
		vector windVelocity = m_pWeatherManager.GetWindSpeed() * vector.FromYaw(m_pWeatherManager.GetWindDirection());
		float horizontalProgress = Math.Pow(Math.E, -m_fHorizontalRateConstant * timeSlice);
		v[0] = windVelocity[0] + (v[0] - windVelocity[0]) * horizontalProgress;
		v[1] = m_fTerminalVelocity + (v[1] - m_fTerminalVelocity) * Math.Pow(Math.E, -m_fVerticalRateConstant * timeSlice);
		v[2] = windVelocity[2] + (v[2] - windVelocity[2]) * horizontalProgress;
		GetPhysics().SetVelocity(v);
		GetPhysics().SetAngularVelocity(vector.Zero);
		HandleLanding(owner, timeSlice);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Handle landing
	protected void HandleLanding(IEntity owner, float timeSlice)
	{
		m_pTraceParam.Start = GetOrigin() + 0.25 * vector.Up;
		m_pTraceParam.End = m_pTraceParam.Start - 0.5 * vector.Up;
		
		if (m_pWorld.TraceMove(m_pTraceParam, TraceFloorFilter) < 1)
			Terminate();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Ignore plants
	protected bool TraceFloorFilter(notnull IEntity entity)
	{
		return !Tree.Cast(entity);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set inital morph state
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (GetGame().InPlayMode())
			GetAnimation().SetMorphState("Opening", m_fMorphProgress);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Handle morphing on each client
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fMorphProgress = Math.Clamp(m_fMorphProgress + m_fMorphRate * timeSlice, 0, 1);
		GetAnimation().SetMorphState("Opening", m_fMorphProgress);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Terminate()
	{
		ClearEventMask(EntityEvent.SIMULATE | EntityEvent.FIXEDFRAME);
		GetPhysics().SetVelocity(vector.Zero);
		GetPhysics().SetAngularVelocity(vector.Zero);
		GetPhysics().ChangeSimulationState(SimulationState.NONE);
		m_pPerformer.GetPhysics().SetActive(ActiveState.ACTIVE);
		
		//! Tell clients that chute should close
		m_fMorphRate = m_fClosingMorphRate;
		Replication.BumpMe();
		
		super.Terminate();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ToggleFrame(bool enable)
	{
		m_bIsFrameToggledOn = enable;
		OnToggleFrame();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnToggleFrame()
	{
		if (m_bIsFrameToggledOn)
			SetEventMask(EntityEvent.FRAME);
		else
			ClearEventMask(EntityEvent.FRAME);
	}
}
