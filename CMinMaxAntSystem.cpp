#include "StdAfx.h"


#include "CMinMaxAntSystem.h"


//************************************
// Method:    CMinMaxAntSystem
// FullName:  CMinMaxAntSystem::CMinMaxAntSystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , Vdistance)
// Parameter: Parameters & Par
// Parameter: std::vector<std::vector<int> > * Vdistance
//************************************
CMinMaxAntSystem::CMinMaxAntSystem(Parameters& Par , std::vector<std::vector<int> >  *Vdistance)  	
	: CAntSystem(Par , Vdistance) , m_restartAntBestPath(m_noNodes)
{
	 m_restartBestAntTourLength =  (std::numeric_limits<int>::max)();
	 lambda = Par.lambda ; // 0.05;
	 m_resetAnt =false;
}
//************************************
// Method:    chooseClosestNext
// FullName:  CMinMaxAntSystem::chooseClosestNext
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: std::vector<bool> & antsvisted
// Parameter: std::vector<size_t> & nntour
//************************************
void CMinMaxAntSystem::chooseClosestNext(std::vector<bool> &antsvisted , std::vector<size_t> &nntour)
{
	size_t next_city =this->m_noNodes;
	for(size_t phase = 1; phase < m_noNodes; phase++)
	{
		size_t current_city = nntour[phase - 1];
		size_t min_distance = (std::numeric_limits<int>::max)();
		
		for (size_t city = 0; city < m_noNodes; city++)
		{
			if (!antsvisted[city])
				if ((*m_distanceMatrix)[current_city][city] < min_distance)
				{
					next_city = city;
					min_distance = (*m_distanceMatrix)[current_city][city];
				}
		}
		nntour[phase] = next_city;
		antsvisted[next_city] = true;
	}
}
 //************************************
 // Method:    nodeBranching
 // FullName:  CMinMaxAntSystem::nodeBranching
 // Access:    private 
 // Returns:   double
 // Qualifier:
 // Parameter: double l
 //************************************
double CMinMaxAntSystem::nodeBranching(double l) const
{
	return this->m_newPheromoneMatrix->average_lambda_branching_factor(l);
}
//************************************
// Method:    updateBestSoFarPath
// FullName:  CMinMaxAntSystem::updateBestSoFarPath
// Access:    virtual public 
// Returns:   bool
// Qualifier:
//************************************
bool CMinMaxAntSystem::updateBestSoFarPath()
{
		if(!CAntSystem::updateBestSoFarPath())
		{

		}
		//best in itteration
		if (m_BestAntToDate.getAntTourLength() < m_restartBestAntTourLength)
		{
				
			m_restartAntBestPath = m_pBestAntItt->getAntsCityTour();
			m_restartBestAntTourLength = m_pBestAntItt->getAntTourLength(); calculatePathLength(m_restartAntBestPath);// m_bestSoFarPathlength;
			irreationSinceLastBest =0;
		}
		else 
		{
		
			//reset trails
		//	m_bestIterationLength = m_BestAntToDate.getAntTourLength();
			m_branchingFactor = this->nodeBranching(lambda);
			double p_x = exp(log(0.05) / m_noNodes);
			trail_min = 1. * (1. - p_x) / (p_x * (double) ((this->m_noNodes + 1) / 2));
			trail_max = 1. / ((m_rho) * m_BestAntToDate.getAntTourLength());
			trail_0 = trail_max;
			trail_min = trail_max * trail_min;

		}
		return true;
 }
//************************************
// Method:    initPheromones
// FullName:  CMinMaxAntSystem::initPheromones
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CMinMaxAntSystem::initPheromones()
{
		std::vector<bool> visited(m_noNodes);
		std::vector<size_t> nntour(m_noNodes);
		//calculate min max values inital
		int phase = 0;
		int rnd= (rand()%(visited.size()-1))+1;
		nntour[0] =rnd;
		visited[rnd] = true;
		chooseClosestNext(visited, nntour);
		double distance = this->calculatePathLength( nntour);
		trail_max = 1. / ((m_rho) * distance);
		trail_min = trail_max / (2. * m_noNodes);
		initPheromoneTrails(trail_max);
}	
//************************************
// Method:    initPheromoneTrails
// FullName:  CMinMaxAntSystem::initPheromoneTrails
// Access:    private 
// Returns:   void
// Qualifier: const
// Parameter: double initialValue
//There are variants in the selection of the ants allowed to update pheromones: the best to date ant, 
//or the best for current iteration, or the best after latest reset ant, or the best to date ant for even iterations, 
//and the best for iteration for odd iterations.
//************************************
void CMinMaxAntSystem::initPheromoneTrails(double initialValue) const
 {
	 for (size_t i = 0; i < m_noNodes; i++)
	 {
		 for (size_t j = 0; j <= i; j++) 
		 {
			 m_newPheromoneMatrix->set(i , j , initialValue);
		 }
	 }

	 for(size_t i = 0; i < m_noNodes; i++)
		 m_newPheromoneMatrix->set(i , i , 0);
}
//************************************
// Method:    globalUpdatePheromone
// FullName:  CMinMaxAntSystem::globalUpdatePheromone
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: const std::vector<size_t> & AntTour
//************************************
void CMinMaxAntSystem::globalUpdatePheromone(const std::vector<size_t> &AntTour)
{
	int Q=1;
	double tour_length = calculatePathLength (AntTour); 
	for(size_t city = 1; city < AntTour.size(); city++)
	{
		size_t from = AntTour[city-1];
		size_t to = AntTour[city];
 		// eq 14.2 / 14.3
		double tpadd = (Q/tour_length);
		m_newPheromoneMatrix->add(from , to , tpadd);
	}
}
//************************************
// Method:    checkPheromoneLimits
// FullName:  CMinMaxAntSystem::checkPheromoneLimits
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CMinMaxAntSystem::checkPheromoneLimits()
{
	for (size_t i = 0; i < m_noNodes ; i++) 
	{
		for (size_t j = 0; j < i; j++) 
		{
			double value =  m_newPheromoneMatrix->get(i,j);
			if ( value < trail_min)
				m_newPheromoneMatrix->set(i,j , trail_min);
			else if ( value > trail_max )
				m_newPheromoneMatrix->set(i,j , trail_max);
		}
	}
}
//************************************
// Method:    updatePheromones
// FullName:  CMinMaxAntSystem::updatePheromones
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CMinMaxAntSystem::updatePheromones()
{
	//end of ants walk
	evaporateAllPhero ();	
	if (this->m_iterations % 25 == 0) //even
	{
		globalUpdatePheromone(m_BestAntToDate.getAntsCityTour());  //best to date for even
	} 
	else //odd
	{
	    if(m_resetAnt)
		{
			globalUpdatePheromone(m_restartAntBestPath);
			m_resetAnt = false;
		}
	    else
		{
			globalUpdatePheromone(m_pBestAntItt->getAntsCityTour());
		}
	}
				
	if ( irreationSinceLastBest > 150) 	//if (irreationSinceLastBest > 100) 
	{
		m_branchingFactor = nodeBranching(lambda);
		double  branch_fac = 1.00001;
		if ( (m_branchingFactor < branch_fac)) 
		{
			m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());
			initPheromoneTrails(trail_max);
			m_resetAnt = true;
			irreationSinceLastBest =0;
		}

	}
	checkPheromoneLimits();
	calculateHeuristicMatrix();
   
}
//************************************
// Method:    ~CMinMaxAntSystem
// FullName:  CMinMaxAntSystem::~CMinMaxAntSystem
// Access:    virtual public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
CMinMaxAntSystem::~CMinMaxAntSystem(void)
{
	
}
