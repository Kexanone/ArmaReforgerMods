//------------------------------------------------------------------------------------------------
class AF_AvalancheFrontEntityClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Avalanche front is basically a physical boulder with a particle emitter attached
//!
//! Since particles will currently despawn when the emitter gets too far, the emitters have to be
//! replaced and left behind as a workaround.
//!
//! Area of death is rendered with lines in workbench.
class AF_AvalancheFrontEntity : GenericEntity
{
	[Attribute(desc: "Emitter class name attached to front")]
	protected ResourceName m_sEmitterName;
	
	[Attribute(defvalue: "3", desc: "Delay in seconds before the first emitter spawns")]
	protected float m_fEmitterSpawnDelay;
	
	[Attribute(defvalue: "5", desc: "Minmum speed in m/s before the front despawns")]
	protected float m_fMinimumSpeed;
	
	protected ParticleEffectEntity m_pPreviousEmitter;
	protected SoundComponent m_pSoundComponent;
	protected ref array<vector> m_aTrajectory = {};
	protected ref array<vector> m_aPerps = {};
	protected vector m_vCurrentVelocity;
	protected ref array<vector> polygon = {};
	protected float m_fMaxParticleRadius = 15;
	
#ifdef WORKBENCH
	protected vector m_aDebugLines[6];
#endif
	
	//------------------------------------------------------------------------------------------------
	void AF_AvalancheFrontEntity(IEntitySource src, IEntity parent)
	{
		polygon.Resize(5);
		GetGame().GetCallqueue().CallLater(InitializeFrontHandler, m_fEmitterSpawnDelay*1000);
		
	#ifdef WORKBENCH
		SetEventMask(EntityEvent.FRAME);
	#endif
	}
	
	//------------------------------------------------------------------------------------------------
	protected void InitializeFrontHandler()
	{
		m_pSoundComponent = SoundComponent.Cast(FindComponent(SoundComponent));
		if (m_pSoundComponent)
			m_pSoundComponent.SetSignalValueStr("Trigger", 1);
		
		GetGame().GetCallqueue().CallLater(FrontHandler, Math.RandomIntInclusive(1000, 1500), true);
		FrontHandler();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FrontHandler()
	{
		m_vCurrentVelocity = GetPhysics().GetVelocity();
		
		if (Replication.IsServer() || !Replication.IsRunning())
		{
			// Save trajectory and perpendicular vector
			m_aTrajectory.Insert(GetOrigin());
			vector dir = m_vCurrentVelocity;
			dir[1] = 0;
			dir.Normalize();
			m_aPerps.Insert(Vector(dir[2], 0, -dir[0]));
			
			HandleKillArea();
		}

		// Stop front when advancing too slow
		if (m_vCurrentVelocity.Length() <  m_fMinimumSpeed)
		{
			DetachCurrentEmitter();
			
			if (m_pSoundComponent)
				m_pSoundComponent.SetSignalValueStr("Trigger", 0);
			
			GetGame().GetCallqueue().Remove(FrontHandler);
			
			// Schedule clean-up on the server
			if (Replication.IsServer() || !Replication.IsRunning())
				GetGame().GetCallqueue().CallLater(SCR_EntityHelper.DeleteEntityAndChildren, 30000, false, this);
			
			return;
		};
		
		// Workaround: Emitters needs to be replaced regularly, since particle effects would disappear
		// when its emitter is too far from it
		DetachCurrentEmitter();
		CreateAndAttachEmitter();
		
	#ifdef WORKBENCH
		for (int i = 0; i < 5; i++)
			m_aDebugLines[i] = polygon[i];
		
		m_aDebugLines[5] = m_aDebugLines[0];
	#endif
	}
	
	//------------------------------------------------------------------------------------------------
	//! Kill area construction:
	//!
	//! traj[-4] x---o---x
	//!			 |   |   |
	//! traj[-3] x   o   x
	//!			  \  |  /
	//! traj[-2]   \ o /
	//!				\|/
	//! traj[-1]     o
	//!
	//! o: Points of the trajectory
	//! x: Points on the line perpendicular to velocity projected on XZ with a distance of
	//!    max particle radius from the trajectory point
	protected void HandleKillArea() 
	{
		// We need at least 4 points in our trajectory to construct the kill area
		int count = m_aTrajectory.Count();
		if (count < 4)
			return;

		polygon[0] = m_aTrajectory[count-1];
		
		for (int i = 0; i < 2; i++)
		{
			vector pos = m_aTrajectory[count-3-i];
			vector perp = m_aPerps[count-3-i];
			polygon[1+i] = pos + m_fMaxParticleRadius * perp;
			polygon[4-i] = pos - m_fMaxParticleRadius * perp;
		};
		
		// Query radius is either the distance between front and mid point or the radius for circumscribing a circle about a square which circumscribes the maximum size of the particle
		float queryRadius = Math.Max(vector.Distance(m_aTrajectory[count-1], m_aTrajectory[count-3]), Math.Sqrt(2)/2*m_fMaxParticleRadius);
		GetGame().GetWorld().QueryEntitiesBySphere(m_aTrajectory[count-3], queryRadius, QueryEntityInKillZone);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool QueryEntityInKillZone(IEntity entity) 
	{
		if (entity.GetParent())
			return true;
		
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(entity.FindComponent(SCR_DamageManagerComponent));
		if (!damageManager || damageManager.IsDestroyed())
			return true;
		
		if (!Math2D.IsPointInPolygonXZ(polygon, entity.GetOrigin()))
			return true;
		
		damageManager.Kill(Instigator.CreateInstigator(null));
		
		Physics physics = entity.GetPhysics();
		if (physics)
			physics.SetVelocity(m_vCurrentVelocity);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CreateAndAttachEmitter()
	{
		m_pPreviousEmitter = ParticleEffectEntity.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sEmitterName)));
		AddChild(m_pPreviousEmitter, -1);
		vector transform;
		Math3D.MatrixIdentity4(transform);
		m_pPreviousEmitter.SetLocalTransform(transform);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DetachCurrentEmitter()
	{
		if (m_pPreviousEmitter)
		{
			RemoveChild(m_pPreviousEmitter, true);
			m_pPreviousEmitter.StopEmission();
		};
	}
	
#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		Shape.CreateLines(
			Color.RED,
			ShapeFlags.ONCE | ShapeFlags.NOZWRITE,
			m_aDebugLines,
			6
		);
	}
#endif
}