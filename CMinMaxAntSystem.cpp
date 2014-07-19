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
	 m_lambda = Par.lambda ; // 0.05;
	 m_resetAnt =false;
}
 //************************************
 // Method:    nodeBranching
 // FullName:  CMinMaxAntSystem::nodeBranching
 // Access:    private 
 // Returns:   double
 // Qualifier:
 // Parameter: double l
 //************************************
inline double CMinMaxAntSystem::nodeBranching(double l) const
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
			double branchingFactor = this->nodeBranching(m_lambda);
			double p_x = exp(log(0.05) / m_noNodes);
			m_trail_min = 1. * (1. - p_x) / (p_x * (double) ((this->m_noNodes + 1) / 2));
			m_trail_max = 1. / ((m_rho) * m_BestAntToDate.getAntTourLength());
			m_trail_0 = m_trail_max;
			m_trail_min = m_trail_max * m_trail_min;

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
	std::vector<size_t>  randomPath = m_pLocalSearch->greedyPath(m_noNodes);
	m_BestAntToDate.setAntsTour(randomPath);
	
	if(m_LocalSearchOpt2)
		m_pLocalSearch->opt2(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchOpt3)
		m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchGreed)
		m_pLocalSearch->three_opt_first(m_BestAntToDate.getAntsCityTour() , m_nnList);
					
	m_BestAntToDate.setAntTourLength(this->calculateAntPathLength(m_BestAntToDate));
	m_trail_max = 1. / ((m_rho) * m_BestAntToDate.getAntTourLength());
	m_trail_min = m_trail_max / (2. * m_noNodes);
	initPheromoneTrails(m_trail_max);
	calculateHeuristicMatrix();
	
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
	double Q=1.0;
	double tour_length = calculatePathLength (AntTour); 
	for(size_t city = 1; city < AntTour.size(); city++)
	{
		size_t from = AntTour[city-1];
		size_t to = AntTour[city];
 		// eq 14.2 / 14.3
		double tpadd = (double) (Q/tour_length);
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
			if ( value < m_trail_min)
				m_newPheromoneMatrix->set(i,j , m_trail_min);
			else if ( value > m_trail_max )
				m_newPheromoneMatrix->set(i,j , m_trail_max);
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
	evaporateAllPhero();	
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
		double branchingFactor = nodeBranching(m_lambda);
		double  branch_fac = 1.00001;
		if ( (branchingFactor < branch_fac)) 
		{
			m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());
			initPheromoneTrails(m_trail_max);
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
