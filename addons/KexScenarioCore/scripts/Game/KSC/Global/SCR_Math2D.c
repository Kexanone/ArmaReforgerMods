//------------------------------------------------------------------------------------------------
//! SCR_Math2D Class
//!
//! Contains various scripted 2D math functions
//------------------------------------------------------------------------------------------------
modded class SCR_Math2D
{
	//------------------------------------------------------------------------------------------------
	// TODO: better
	/*!
		Get a random point within the provided 2D polygon
		\param polygon
		\param x
		\param x
		\return true on success, false otherwise
	*/
	override static bool GetRandomPointInPolygon(notnull array<float> polygon, out float x, out float y)
	{
		float minX, minY, maxX, maxY;
		if (!GetMinMaxPolygon(polygon, minX, maxX, minY, maxY))
			return false;

		GetRandomPointInRectangle(minX, maxX, minY, maxY, x, y);
		while (!Math2D.IsPointInPolygon(polygon, x, y)) // ugh
		{
			GetRandomPointInRectangle(minX, maxX, minY, maxY, x, y);
		}

		return true;
	}
};