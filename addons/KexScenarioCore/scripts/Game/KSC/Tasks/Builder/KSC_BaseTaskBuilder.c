//------------------------------------------------------------------------------------------------
class KSC_BaseTaskBuilder : ScriptAndConfig
{
	[Attribute(desc: "Prefab name of the task entity to be spawned", uiwidget: UIWidgets.ResourceNamePicker, params: "et")]
	protected ResourceName m_sTaskPrefabName;
	
	//------------------------------------------------------------------------------------------------
	KSC_BaseTask Build();
	
	//------------------------------------------------------------------------------------------------
	protected KSC_BaseTask SpawnTaskEntity(vector pos)
	{
		return KSC_BaseTask.Cast(KSC_GameTools.SpawnPrefab(m_sTaskPrefabName, pos));
	}
}
