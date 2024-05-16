//------------------------------------------------------------------------------------------------
/*
class KSC_GrahamScan
{
	//------------------------------------------------------------------------------------------------
	static array<vector> Compute(array<vector> points)
	{
		if (!points || points.IsEmpty())
			return null;
			
		array<vector> sortedPoints;
		sortedPoints.Copy(points);
		KSC_GrahamScanComparePoints.SetStartVector();
		SCR_Sorting<vector, KSC_GrahamScanComparePoints>.HeapSort(sortedPoints);
		sortedPoints.SwapItems(0, GetStartIdx(sortedPoints));	
		vector startPoint = sortedPoints[0];
		
		// Remove collinear points; Keep the one farther away from the start point
		for (int i = sortedPoints.Count() - 1; i > 1; i--)
		{
			vector left = sortedPoints[i-1] - startPoint;
			left[1] = 0;
			vector right = sortedPoints[i] - startPoint;
			right[1] = 0;
			
			// Skip if points are not collinear
			if (vector.DotXZ(left, right) != 0)
				continue;
			
			// Closer point should be the current index to be removed
			if (left.LengthSq() < right.LengthSq())
				sortedPoints.SwapItems(i-1, i);
			
			sortedPoints.Remove(i);
		}
		
		if (sortedPoints.Count() < 3)
			return null;
		
		KSC_Stack<vector> stack = new KSC_Stack<vector>();
		
			
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	static int GetStartIdx(array<vector> points)
	{
		vector minZPoint = points[0];
		int minZIdx = 0;
		
		foreach (int idx, vector point : points)
		{
			if (minZPoint[2] > point[2])
				continue;
			
			if (minZPoint[2] == point[2] && minZPoint[2] < point[2])
				continue;
			
			minZIdx = idx;
			minZPoint = point;
		}
		
		return minZIdx;
	}
}

//------------------------------------------------------------------------------------------------
class KSC_GrahamScanComparePoints : SCR_SortCompare<vector>
{
	protected vector m_vStart;
	
	//------------------------------------------------------------------------------------------------
	static void SetStartVector(vector v)
	{
		m_vStart = v;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return true if left has a smaller polar angle
	override static int Compare(vector left, vector right)
	{
		vector relLeft = left - m_vStart;
		relLeft[1] = 0;
		float leftAngle = relLeft.ToYaw();
		vector relRight = right - m_vStart;
		relRight[1] = 0;
		float rightAngle = relRight.ToYaw();
		
		if (leftAngle != relRight)
			return (left.ToYaw() < right.ToYaw());
		
		return (relLeft.LengthSq() > relRight.LengthSq());
	}
}
*/