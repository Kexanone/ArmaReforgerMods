//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController : PlayerController
{
	//------------------------------------------------------------------------------------------------
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		
		if (!Replication.IsServer() || !to || m_bIsPossessing)
			return;

		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(to.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
		charCtrl.RSA_MedicalFeed_Initialize(GetPlayerId());
	}
};