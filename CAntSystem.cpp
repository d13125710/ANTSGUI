#include "StdAfx.h"
#include "CAntSystem.h"
#include "resource.h"





//************************************
// Method:    CAntSystem
// FullName:  CAntSystem::CAntSystem
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//************************************
CAntSystem::CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix)
{
	m_distanceMatrix = distanceMatrix;
	Create(Par);
   
}
//************************************
// Method:    Create
// FullName:  CAntSystem::Create
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: Parameters & Par
//************************************
void CAntSystem::Create(Parameters& Par)
{
	this->m_noNodes = m_distanceMatrix->size();
	if(Par.noAnts > m_noNodes)
		Par.noAnts = (int) m_noNodes;
	this->m_noAnts = Par.noAnts;
    this->m_alpha = Par.Alpha;
    this->m_beta = Par.Beta;
    this->m_rho = Par.rho;
	m_LocalSearchOpt2 = Par.LocalSearchOpt2;
	m_LocalSearchOpt3 = Par.LocalSearchOpt3;
	m_LocalSearchGreed = Par.LocalSearchGreed;
	irreationSinceLastBest=0;
	m_RestartBestAnt.setAntTourLength(0);
//	worseindex=-1;
	m_pLocalSearch = new CLocalSearch(m_distanceMatrix);
    this->m_iterations = 0;
	//m_doOpt3
	m_heuristicMatrix= new std::vector<std::vector<double> >(m_noNodes , m_noNodes);
	//test
	m_newPheromoneMatrix = new PheroMatrix(m_noNodes,m_rho);

//	m_bestSoFarPath.resize(m_noNodes+1);
//	m_worstSoFarPath.resize(m_noNodes+1);

	m_Ants.empty();
	m_Ants.resize(m_noAnts-1);
	for(int i = 0; i < m_Ants.size(); i++)
	{
		CAnt newAnt(m_noNodes);
		m_Ants[i] = newAnt;
	}
	
//	m_bestIrrPath.resize(m_noNodes+1);  //refactor
//	m_worstIrrPath.resize(m_noNodes+1);



	m_randomPath.resize(m_noNodes);
	for (size_t i=0; i<m_noNodes; i++)
		m_randomPath[i]=i;
	std::random_shuffle( m_randomPath.begin() , m_randomPath.end() );
	//m_generator((random_dev());
	t_prob.resize(m_noNodes);
	m_strength.resize(m_noNodes+1);
	
	//set the lenths of the paths
//	m_bestSoFarPathlength = (std::numeric_limits<int>::max)();
//	m_worstSoFarPathlength =0;


	//new
	m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());
	m_WorstAntToDate.setAntTourLength(0);
	m_BestAntToDate.setAntTourLength((std::numeric_limits<int>::max)());


}

//************************************
// Method:    updatePheromones
// FullName:  CAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void  CAntSystem::updatePheromones()
{
    evaporateAllPhero();
    for(size_t k = 0; k < m_Ants.size(); k++)
        adjustPheremone(k);
	//dont think i need this on irreration
    calculateHeuristicMatrix();
}

//************************************
// Method:    initMatrix
// FullName:  CAntSystem::initMatrix
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: std::vector<std::vector<int> > & distanceMatrix
//************************************
void CAntSystem::initMatrix(std::vector<std::vector<int> > &distanceMatrix)
{
    size_t i,j;
	this->m_distanceMatrix = &distanceMatrix;
     
	for(i = 0; i < m_noNodes; i++)
		for(j = 0; j < m_noNodes; j++)
		{
			m_newPheromoneMatrix->set(i,j,0.0);
			(*m_heuristicMatrix)[i][j] = 0.0;
		}
}
 void CAntSystem::initPheromones()
{
	
	for (size_t i=0; i<m_noNodes; i++)	{
		for (size_t j=0; j<m_noNodes; j++)
			m_newPheromoneMatrix->set(i,j, (0.1 * rand() / (double)RAND_MAX));
	}
       
}
 void CAntSystem::initAnts()
 {
     for(size_t i = 0; i < m_Ants.size(); i++)
		m_Ants[i].ReinitiseAnt();
//	shuffle(m_Randomnseq.begin(), m_Randomnseq.end(), m_generator);

 }

void CAntSystem::adjustPheremone(size_t antPos)
{
	const CAnt &pant=m_Ants[antPos];
	double tour_length = (double) (1.0 / pant.getAntTourLength());
	//double tour_length = 1 / calculatePathLength(pant.getAntsCityTour()); 
	for(size_t city = 0; city < pant.getNoNodes()-1; city++)
	 {
	      
		size_t from = m_Ants[antPos].getCity(city);
		size_t to   = m_Ants[antPos].getCity(city+1);
		// eq 14.2 / 14.3
		m_newPheromoneMatrix->add(from , to , tour_length );	
  }
//	size_t from = m_Ants[antPos].getCity(m_noNodes-1);
//	size_t to   = m_Ants[antPos].getCity(0);
	// eq 14.2 / 14.3
//	m_newPheromoneMatrix->add(from , to , tour_length );	

}

//************************************
// Method:    decisionRule
// FullName:  CAntSystem::decisionRule
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: size_t k
// Parameter: size_t step
//************************************
void CAntSystem::decisionRule(size_t k, size_t step)
{
	size_t c = m_Ants[k].getCity(step-1); 
	for (size_t i=0; i < m_noNodes; i++) 
	{
		t_prob[i] = 0.0;
		m_strength[i] = 0;	
		if (m_Ants[k].isCityVisited(i) == false)	
		{
			try
			{
				t_prob[i]= (double)(*m_heuristicMatrix)[c][i];
			}
			catch (...)
			{
				TRACE("Exception k=%d , step=%d , i=%d , c=%d" , k , step , i ,c);
			}
			
		}
	}
	for (size_t z =0; z < m_noNodes; z++)
		m_strength[z+1] = t_prob[z] + m_strength[z];
	double x = fRand(0,  m_strength[m_noNodes]);
	int y = 0;
	//while (  y !=m_strength.size() && !((m_strength[y] <= x) && (x <= m_strength[y+1])) )
	while (!((m_strength[y] <= x) && (x <= m_strength[y+1])) )
		y++;

	m_Ants[k].setAntCity(step, y);
	m_Ants[k].setCityVisited(y);

}


//************************************
// Method:    constructSolutions
// FullName:  CAntSystem::constructSolutions
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::constructSolutions()
{
	initAnts();
	   
	for (size_t k = 0; k < m_Ants.size(); k++ )
	{
		m_Ants[k].setAntCity(0,m_randomPath[k]);
		m_Ants[k].setCityVisited(m_randomPath[k]);

	}

	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_Ants.size(); k++)
			decisionRule(k,step);
	}
    //add in the last city to the first city tour ...  
	for(size_t k = 0; k < m_Ants.size(); k++)
	{
			
		size_t tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
	//	const std::vector<size_t>& tourvector = m_Ants[k].getAntsCityTour();
	//	m_Ants[k].setAntTourLength((int)calculatePathLength(tourvector));
		m_Ants[k].setAntTourLength((int)this->calculateAntPathLength(m_Ants[k]));
	}
	updateBestSoFarPath();
}

//************************************
// Method:    getBestTourIrreration
// FullName:  CAntSystem::getBestTourIrreration
// Access:    public 
// Returns:   std::vector<size_t> &
// Qualifier:
//************************************
//std::vector<size_t> &CAntSystem::getBestTourIrreration()
//{
   
	//return m_pBestAntItt->getAntsCityTour();
	//int bestTourLength = (std::numeric_limits<int>::max)();
	//for(size_t j = 0; j < m_noNodes; j++)
	//{
	//	m_bestIrrPath[j] = m_Ants[BestAntIndex].getCity(j);
	//}
    //return m_bestIrrPath;
//}
//************************************
// Method:    getWorstTourIrreration
// FullName:  CAntSystem::getWorstTourIrreration
// Access:    public 
// Returns:   std::vector<size_t> &
// Qualifier:
//************************************
/*
std::vector<size_t> &CAntSystem::getWorstTourIrreration()
{
	int worstTourLength = 0;
	for(size_t j = 0; j < m_noNodes; j++)
	{
		m_worstIrrPath[j] = m_Ants[worseindex].getCity(j);
	}
	return m_worstIrrPath;
}
*/
//************************************
// Method:    evaporateAllPhero
// FullName:  CAntSystem::evaporateAllPhero
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::evaporateAllPhero()
{
	m_newPheromoneMatrix->evaporate_all();
}

//************************************
// Method:    calculatePathLength
// FullName:  CAntSystem::calculatePathLength
// Access:    public 
// Returns:   double
// Qualifier: const
// Parameter: const std::vector<size_t> & currentPath
//************************************
double CAntSystem::calculatePathLength(const std::vector<size_t> &currentPath) const
{
    size_t  start, end;
	double d = 0;
	for ( size_t i=1; i<currentPath.size(); i++)
	{
		start = currentPath[i-1];
		end = currentPath[i];
		d += (*m_distanceMatrix)[start][end];	
	}
	d += (*m_distanceMatrix)[currentPath[m_noNodes-1]][currentPath[0]];
	return d;
}
//************************************
// Method:    calculateHeuristicMatrix
// FullName:  CAntSystem::calculateHeuristicMatrix
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::calculateHeuristicMatrix()
{
	for(size_t i = 0; i < m_noNodes; i++)
		for(size_t j = 0; j <m_noNodes; j++)
		{
			double xx = m_newPheromoneMatrix->get(i,j);
			double yy = (*m_distanceMatrix)[i][j];
			(*m_heuristicMatrix)[i][j] =(double) (yy) ?  abs(pow( xx , m_alpha) / pow(yy,m_beta)) :  abs(pow( xx , m_alpha) / pow(1,m_beta));
		}	
}
//************************************
// Method:    updateBestSoFarPath
// FullName:  CAntSystem::updateBestSoFarPath
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::updateBestSoFarPath()
{

	//find the worst ant , best ant , and
	int pathdistance =(std::numeric_limits<int>::max)();
	int worstdistance = 0;
	
	for(size_t a =0;a<m_Ants.size(); a++)
	{
		int antPathLength = m_Ants[a].getAntTourLength();
		if(antPathLength < pathdistance)
		{
			pathdistance = antPathLength;
			m_pBestAntItt = &m_Ants[a];
		}
		if(antPathLength > worstdistance)
		{
			worstdistance = antPathLength;
			m_pWorstAntItt = &m_Ants[a];
		}
	}
	if(m_pBestAntItt->getAntTourLength() < m_RestartBestAnt.getAntTourLength())
	{
		m_RestartBestAnt = *m_pBestAntItt;
		std::string s =std::string(format("Restart best Ants %d @ %d" , m_RestartBestAnt.getAntTourLength() ,m_iterations));
		static string c;c=s;
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);

	}

	if(m_pWorstAntItt->getAntTourLength() > m_WorstAntToDate.getAntTourLength())
	{
		m_WorstAntToDate = *m_pWorstAntItt;
		std::string s =format("Worst Ant to date found  %f  @ %d",m_WorstAntToDate.getAntTourLength() ,m_iterations);
		static string c; c=s;
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
	}
	if(m_pBestAntItt->getAntTourLength() < m_BestAntToDate.getAntTourLength())
	{
		m_BestAntToDate = *m_pBestAntItt;
		irreationSinceLastBest=0;
	}
	else
	{
		irreationSinceLastBest++;
	}



		
	
	
	/*double distance=  this->calculatePathLength(m_bestSoFarPath);
	//	int bestitter = computeAntsItterationBestIndex() ;
//		if(distance == 0 || this->m_iterations ==0)
//		{
m_bestSoFarPath = getBestTourIrreration();
m_bestSoFarPathlength = computeAntsItterationBestIndex() ;
			irreationSinceLastBest=0;
			m_bestAntToDate = m_Ants[BestAntIndex];
		}
        else if(bestitter < distance)
		{
			irreationSinceLastBest=0;
			m_bestSoFarPathlength = bestitter;
			m_bestSoFarPath = getBestTourIrreration();
			m_bestAntToDate = m_Ants[BestAntIndex];

        }
		else
		{
			irreationSinceLastBest++;
		}

		if(bestitter < m_RestartBestAnt.getAntTourLength() )
		{
			m_RestartBestAnt.setAntTourLength(bestitter);
			m_RestartBestAnt.setAntsTour(getBestTourIrreration());

			std::string s =std::string(format("Restart best Ants %d @ %d" , bestitter ,m_iterations));
			static string c;
			c=s;

			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		}
		//get the worst ant for Itteration
		int worstIrr = this->computeAntsItterationWorstIndexAndLength();


				
		if(worstIrr > m_worstSoFarPathlength)
		{
			m_worstSoFarPathlength = worstIrr;
			//get the wost ant to date;
			m_worstSoFarPath = getWorstTourIrreration();
			std::string s =format("Worst Ant to date found  %f  @ %d",m_worstSoFarPathlength ,m_iterations);
			static string c;
			c=s;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		
		}
		


	*/	




}


//************************************
// Method:    ~CAntSystem
// FullName:  CAntSystem::~CAntSystem
// Access:    virtual public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
CAntSystem::~CAntSystem(void)
{
	delete m_heuristicMatrix;
	delete m_pLocalSearch;
	delete m_newPheromoneMatrix;
}
