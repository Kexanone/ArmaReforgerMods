//------------------------------------------------------------------------------------------------
class KSC_Logging
{
	//------------------------------------------------------------------------------------------------
	//! Print message
	static void PrintMessage(string message, string component = string.Empty, LogLevel logLevel = LogLevel.NORMAL)
	{
		string combinedMessage = "KSC";
		
		if (!component.IsEmpty())
			combinedMessage += " - " + component;
				
		combinedMessage += ": " + message;
		Print(message, logLevel);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Print debug message
	static void PrintDebug(string message, string component = string.Empty)
	{
		PrintMessage(message, component, LogLevel.DEBUG);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Print error message
	static void PrintError(string message, string component = string.Empty)
	{
		PrintMessage(message, component, LogLevel.ERROR);
	}
}
