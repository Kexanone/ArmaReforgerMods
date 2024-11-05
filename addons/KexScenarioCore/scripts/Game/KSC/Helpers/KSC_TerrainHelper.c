//------------------------------------------------------------------------------------------------
class KSC_TerrainHelper
{
	//------------------------------------------------------------------------------------------------
	//! Returns true if the terrain surface is below the water surface
	static bool SurfaceIsWater(vector pos)
	{
		pos[1] = GetGame().GetWorld().GetSurfaceY(pos[0], pos[2]);
		vector outWaterSurfacePoint;
		EWaterSurfaceType outType;
		vector transformWS[4];
		vector obbExtents;
		ChimeraWorldUtils.TryGetWaterSurface(GetGame().GetWorld(), pos, outWaterSurfacePoint, outType, transformWS, obbExtents);
		return outType != EWaterSurfaceType.WST_NONE;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns the terrain slope in degrees for the given position
	static float GetSlope(vector pos)
	{
		vector normal = SCR_TerrainHelper.GetTerrainNormal(pos);
		return Math.RAD2DEG * Math.Acos(vector.Dot(Vector(0, 1, 0), normal));
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if the terrain slope at the given position is below the specified angle
	//! <maxAngle> is given in degrees
	static bool SurfaceIsFlat(vector pos, float maxAngle = 5)
	{
		return GetSlope(pos) < maxAngle;
	}
	
	//------------------------------------------------------------------------------------------------
	static ResourceName GetSurfaceMaterial(vector pos)
	{
		pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
		TraceParam params = new TraceParam();
		params.Flags = TraceFlags.WORLD;
		params.Start = pos + 0.01 * vector.Up;
		params.End = pos - 0.01 * vector.Up;
		GetGame().GetWorld().TraceMove(params, null);
		return params.TraceMaterial;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SurfaceIsRoad(vector pos)
	{
		ResourceName material = GetSurfaceMaterial(pos);
		return material.IndexOf("Road_") >= 0;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SurfaceIsRunway(vector pos)
	{
		ResourceName material = GetSurfaceMaterial(pos);
		return material.IndexOf("Runway") >= 0;
	}
}
