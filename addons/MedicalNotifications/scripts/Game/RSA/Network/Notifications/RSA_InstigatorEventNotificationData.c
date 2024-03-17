enum RSA_EInstigatorEvent
{
	INCAPACITATED,
	REVIVED
};

[BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(RSA_EInstigatorEvent, "m_InstigatorEventKey")]
class RSA_InstigatorEventNotificationKeys
{	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(RSA_EInstigatorEvent))]
	RSA_EInstigatorEvent m_InstigatorEventKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_UnknownOnPlayerNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_UnknownOnPossessedNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_PlayerOnPlayerNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_PlayerOnPossessedNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_PossessedOnPlayerNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_PossessedOnPossessedNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_AIOnPlayerNotificationKey;
	
	[Attribute("0", UIWidgets.SearchComboBox, "", "", ParamEnumArray.FromEnum(ENotification))]
	ENotification m_AIOnPossessedNotificationKey;
};
