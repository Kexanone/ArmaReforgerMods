//------------------------------------------------------------------------------------------------
//! Methods for requesting deletion of loadtime entities from a client
modded class SCR_PlayerController : PlayerController
{
	//------------------------------------------------------------------------------------------------
	//! Request deletion of loatime entity at runtime
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void TRA_RpcAsk_DeleteEntityPosition(vector pos)
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.TRA_DeleteEntityPositionsGlobal({pos});
	};
};
