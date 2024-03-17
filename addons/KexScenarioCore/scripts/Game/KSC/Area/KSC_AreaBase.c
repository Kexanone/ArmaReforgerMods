//------------------------------------------------------------------------------------------------
class KSC_AreaBase : Managed
{
	//------------------------------------------------------------------------------------------------
	bool IsPointInArea(vector pos) { return false; }
	
	//------------------------------------------------------------------------------------------------
	static vector SamplePointInArea(KSC_AreaBase outer, KSC_AreaBase inner)
	{
		vector pos;
		
		while(!pos || inner.IsPointInArea(pos))
			pos = outer.SamplePointInArea();
		
		return pos;
	}
	
	//------------------------------------------------------------------------------------------------
	vector SamplePointInArea() { return vector.Zero; }
	
	//------------------------------------------------------------------------------------------------
	void GetBoundBox(out vector mins, out vector maxs) {}
}
