//------------------------------------------------------------------------------------------------
//! Creating derived classes from templates does not work yet, otherwise we would rather have
//! KSC_ClusteringBase<Class Features> as parent
//! The Features class requires the static method Features.Distance(Feature x1, Feature x2) to be implemented
class KSC_DBSCAN<Class Features> : Managed
{
	protected float m_fEps;
	protected int m_iMinSamples;
	protected ref array<int> m_aLabels;
	protected int m_iNumLabels;
	
	//------------------------------------------------------------------------------------------------
	void KSC_DBSCAN(float eps = 0.5, int minSamples = 5)
	{
		m_fEps = eps;
		m_iMinSamples = minSamples;
	}
		
	//------------------------------------------------------------------------------------------------
	//! Preforms DBSCAN on the given samples and returns the instance of the model
	//! \param[in] X is the array of the samples
	//! \return array of integers, which represent the cluster labels
	KSC_DBSCAN<Features> Fit(array<Features> X)
	{
		m_aLabels = {};
		m_aLabels.Reserve(X.Count());
		
		for (int i = 0; i < X.Count(); i++)
		{
			m_aLabels.Insert(-1);
		}
		
		m_iNumLabels = 0;
		
		for (int idx = 0; idx < X.Count(); idx++)
		{
			// Skip sample if it already has been assigned to a cluster
			if (m_aLabels[idx] >= 0)
				return this;
			
			ExpandCluster(idx, X);
		}
		
		return this;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Preforms DBSCAN on the given samples and returns the cluster labels
	//! \param[in] X is the array of the samples
	//! \return array of integers, which represent the cluster labels
	array<int> FitPredict(array<Features> X)
	{
		Fit(X);
		return m_aLabels;
	}
	
	//------------------------------------------------------------------------------------------------
	array<int> GetLabels()
	{
		return m_aLabels;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetNumClusters()
	{
		return m_iNumLabels;
	}
	
	//------------------------------------------------------------------------------------------------
	//! \param[in] idx is the index of the sample from which the expansion is started
	//! \param[in] X is the array of the samples
	protected void ExpandCluster(int idx, array<Features> X)
	{
		KSC_Queue<int> samplesToProcess = new KSC_Queue<int>();
		samplesToProcess.Push(idx);
		bool initialSampleAssignedToCluster = false;
		
		while (!samplesToProcess.IsEmpty())
		{
			idx = samplesToProcess.Pop();
			
			array<int> neighbors = GetNeighbors(idx, X);
			// Do not add neighbors if it's not a core point
			if (neighbors.Count() < m_iMinSamples - 1)
				continue;
			
			foreach (int neighbor : neighbors)
			{
				// Only process samples that haven't been handled yet
				if (m_aLabels[neighbor] >= 0)
					continue;
				
				m_aLabels[neighbor] = m_iNumLabels;
				samplesToProcess.Push(neighbor);
			}
			
			if (!initialSampleAssignedToCluster)
			{
				m_aLabels[idx] = m_iNumLabels;
				initialSampleAssignedToCluster = true;
			}
		}
		
		if (initialSampleAssignedToCluster)
			m_iNumLabels++;
	}
	
	//------------------------------------------------------------------------------------------------
	//! \param[in] i1 is the index of the sample for which the neighbors should be computed
	//! \param[in] X is the array of all samples
	//! \return array that contains the indices of all neighbors
	protected array<int> GetNeighbors(int i1, array<Features> X)
	{
		Features x1 = X[i1];
		
		array<int> neighbors = {};
		
		for (int i2 = 0; i2 < X.Count(); i2++)
		{
			if (i1 == i2)
				continue;
			
			if (Features.Distance(x1, X[i2]) <= m_fEps)
				neighbors.Insert(i2);
		}
		
		return neighbors;
	}
}
