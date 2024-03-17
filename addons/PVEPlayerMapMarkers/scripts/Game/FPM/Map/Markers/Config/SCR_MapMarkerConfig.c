//------------------------------------------------------------------------------------------------
//! Base marker types, set in SCR_MapMarkerConfig
//! They are used to differentiate markers in order to easily configure them and reuduce toll on the replication
//! If you have multiple of similar marker types, you should add a single enum here and diffenrentiate the subtypes based on secondaryID (see SCR_MarkerMilitaryEntry)
modded enum SCR_EMapMarkerType
{
	FPM_PLAYER		// dynamic player marker 
}
