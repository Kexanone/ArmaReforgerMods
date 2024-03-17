//------------------------------------------------------------------------------------------------
class AF_AvalancheTriggerEffect : BaseProjectileEffect
{
	[Attribute(defvalue: "{08ECAD6B42C03095}Particles/Blasts/Blast_tiny_snow.ptc", desc: "Resource name of tiny blast effect that can trigger avalanches", category: "Avalanche")]
	protected ResourceName m_sBlastTinyEffectName;
	
	//------------------------------------------------------------------------------------------------
	override void OnEffect(IEntity pHitEntity, inout vector outMat[3], IEntity damageSource, notnull Instigator instigator, string colliderName, float speed)
	{
		super.OnEffect(pHitEntity, outMat, damageSource, instigator, colliderName, speed);
				
		if (Replication.IsRunning() && !Replication.IsServer())
			return;
		
		TraceParam param = new TraceParam();
		param.Start = outMat[0] + vector.Up / 2;
		param.End = outMat[0] - vector.Up / 2;
		param.Flags = TraceFlags.WORLD;
		if (GetGame().GetWorld().TraceMove(param, null) >= 1)
			return;
			
		GameMaterial terrainMaterial = param.SurfaceProps;
		if (!terrainMaterial || terrainMaterial.GetParticleEffectInfo().GetBlastResource(0) != m_sBlastTinyEffectName)
			return;
		
		SCR_BaseGameMode.Cast(GetGame().GetGameMode()).AF_LaunchAvalanche(outMat[0]);
	}
}
