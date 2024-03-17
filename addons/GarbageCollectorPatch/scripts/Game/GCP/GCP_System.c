/*
//------------------------------------------------------------------------------------------------
class GCP_System
{
	protected static float s_fCharacterLifetime = 300;
	protected static float s_fVehicleLifetime = 300;
	protected static float s_fItemLifetime = 120;
	protected static const float s_fTickTimeout = 2;
	protected static int s_iIterationsPerTick = 32;
	protected static ref SCR_SortedArray<IEntity> s_aQueue = new SCR_SortedArray<IEntity>();
	
	//------------------------------------------------------------------------------------------------
	static void Flush()
	{
		for (int i = s_aQueue.Count() - 1; i >= 0; i--)
		{
			DeleteEntityAt(i);
		}
	}
	
	static void Tick()
	{
		for (int i = 0; i < s_iIterationsPerTick; i++)
		{
			if (s_aQueue.IsEmpty())
				return;
			
			if (s_aQueue.GetOrder(0) >= GetGame().GetWorld().GetWorldTime())
				continue;
			
			DeleteEntityAt(0);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected static void DeleteEntityAt(int i)
	{
		IEntity entity = s_aQueue.GetValue(i);
		if (entity && !(entity.GetEventMask() & EntityEvent.FRAME))
			SCR_EntityHelper.DeleteEntityAndChildren(entity);
			
		s_aQueue.Remove(i);
	}
	
	//------------------------------------------------------------------------------------------------
	static void Insert(IEntity entity, float lifetime = -1)
	{
		// Only insert entities that are at the root of hierarchies
		if (!Replication.IsServer() || entity.GetParent())
			return;
		
		if (lifetime < 0)
		{
			lifetime = s_fItemLifetime * 1000;
			
			if (SCR_ChimeraCharacter.Cast(entity))
				lifetime = s_fCharacterLifetime * 1000;
			else if (Vehicle.Cast(entity))
				lifetime = s_fVehicleLifetime * 1000;
		};
		
		s_aQueue.Insert(GetGame().GetWorld().GetWorldTime() + lifetime, entity);
	}
	
	//------------------------------------------------------------------------------------------------
	static void Withdraw(IEntity entity)
	{
		if (!Replication.IsServer())
			return;
		
		s_aQueue.RemoveValues(entity);
	}
	
	//------------------------------------------------------------------------------------------------
	static float GetRemainingLifetime(IEntity entity)
	{
		int i = s_aQueue.Find(entity);
		
		if (i >= 0)
			return s_aQueue.GetOrder(i) - GetGame().GetWorld().GetWorldTime();
		
		return -1;
	}
	
	//------------------------------------------------------------------------------------------------
	static void SetCharacterLifetime(float lifetime)
	{
		s_fCharacterLifetime = lifetime;
	}
	
	//------------------------------------------------------------------------------------------------
	static void SetVehicleLifetime(float lifetime)
	{
		s_fVehicleLifetime = lifetime;
	}
	
	//------------------------------------------------------------------------------------------------
	static void SetItemLifetime(float lifetime)
	{
		s_fItemLifetime = lifetime;
	}
	
	//------------------------------------------------------------------------------------------------
	static void SetIterationsPerTick(int n)
	{
		s_iIterationsPerTick = n;
	}
	
	//------------------------------------------------------------------------------------------------
	static float GetTickTimeout()
	{
		return s_fTickTimeout;
	}
}
*/