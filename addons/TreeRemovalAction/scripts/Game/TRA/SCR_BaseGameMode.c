//------------------------------------------------------------------------------------------------
//! Methods for deleting loadtime entities

modded class SCR_BaseGameMode : BaseGameMode
{
	[RplProp(onRplName: "TRA_DeleteInitialEntityPositions")]
	protected ref array<vector> m_TRA_DeletedEntityPositions = {};
	protected IEntity m_eTRA_NearestEntity;
	protected vector m_TRA_SearchPos;
	protected float m_fTRA_NearestDistanceSq;
	protected const float TRA_SEARCH_DISTANCE = 0.01;
	
	//------------------------------------------------------------------------------------------------
	void TRA_LoadDeletedEntityPositions(array<vector> deletedPositions)
	{
		TRA_DeleteEntityPositionsGlobal(deletedPositions);
		m_TRA_DeletedEntityPositions = deletedPositions;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Ensures that already deleted loadtime entities are deleted for JIPs
	void TRA_DeleteInitialEntityPositions()
	{
		TRA_DeleteEntityPositionsLocal(m_TRA_DeletedEntityPositions);
	};
	
	//------------------------------------------------------------------------------------------------
	//! Deletes loadtime entities for all machines
	//! Can only be called on the server
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void TRA_DeleteEntityPositionsGlobal(array<vector> entityPositions)
	{
		m_TRA_DeletedEntityPositions.InsertAll(entityPositions);
		TRA_DeleteEntityPositionsLocal(entityPositions);
		Rpc(TRA_DeleteEntityPositionsLocal, entityPositions);
	};

	//------------------------------------------------------------------------------------------------
	//! Deletes loadtime entities for local machine
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void TRA_DeleteEntityPositionsLocal(array<vector> entityPositions)
	{
		foreach (vector pos : entityPositions)
		{
			IEntity entity = TRA_GetNearestEntity(pos);
			if (entity)
				SCR_EntityHelper.DeleteEntityAndChildren(entity);
		};
	};

	//------------------------------------------------------------------------------------------------
	//! Return all deleted loatime entities
	//! Can only be called on the server
	array<vector> TRA_GetDeletedEntityPositions()
	{
		return m_TRA_DeletedEntityPositions;
	};
	
	protected IEntity TRA_GetNearestEntity(vector pos)
	{
		m_TRA_SearchPos = pos;
		m_fTRA_NearestDistanceSq = Math.Pow(TRA_SEARCH_DISTANCE, 2);
		m_eTRA_NearestEntity = null;
		GetGame().GetWorld().QueryEntitiesBySphere(m_TRA_SearchPos, TRA_SEARCH_DISTANCE, TRA_EntitySearchCallback);
		return m_eTRA_NearestEntity;
	};
	
	//------------------------------------------------------------------------------------------------
	//! Currently have to filter for closest entity, since QueryEntitiesBySphere is bugged
	//! and somtimes queries entities outside the search distance
	protected bool TRA_EntitySearchCallback(IEntity entity)
	{
		float distanceSq = vector.DistanceSq(m_TRA_SearchPos, entity.GetOrigin());
		
		if (m_fTRA_NearestDistanceSq > distanceSq)
		{
			m_fTRA_NearestDistanceSq = distanceSq;
			m_eTRA_NearestEntity = entity;
		};

		return (m_fTRA_NearestDistanceSq != 0);
	};
};
