//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class TRA_DeleterStruct : SCR_JsonApiStruct
{
	protected ref array<ref TRA_DeletedEntityPosStruct> m_aDeletedEntityPositionStructs = {};
	
	//------------------------------------------------------------------------------------------------
	void TRA_DeleterStruct()
	{
		RegV("m_aDeletedEntityPositionStructs");
	}
	
	//------------------------------------------------------------------------------------------------
	override bool Serialize()
	{
		foreach (vector pos : SCR_BaseGameMode.Cast(GetGame().GetGameMode()).TRA_GetDeletedEntityPositions())
		{
			TRA_DeletedEntityPosStruct posStruct = new TRA_DeletedEntityPosStruct();
			posStruct.SetPosition(pos);
			m_aDeletedEntityPositionStructs.Insert(posStruct);
		};
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool Deserialize()
	{
		array<vector> deletedEntityPositions = {};
		
		foreach (TRA_DeletedEntityPosStruct posStruct : m_aDeletedEntityPositionStructs)
		{
			deletedEntityPositions.Insert(posStruct.GetPosition());
		};
		
		SCR_BaseGameMode.Cast(GetGame().GetGameMode()).TRA_LoadDeletedEntityPositions(deletedEntityPositions);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void Clear()
	{
		m_aDeletedEntityPositionStructs.Clear();
	}
}

//------------------------------------------------------------------------------------------------
class TRA_DeletedEntityPosStruct : SCR_JsonApiStruct
{
	protected vector m_vPos;
	
	//------------------------------------------------------------------------------------------------
	void TRA_DeletedEntityPosStruct()
	{
		RegV("m_vPos");
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetPosition()
	{
		return m_vPos;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPosition(vector pos)
	{
		m_vPos = pos;
	}
}