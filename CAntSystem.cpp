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
CAntSystem::CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix) : 
	m_distanceMatrix(distanceMatrix) , clog("filename.data" ,Par)
{
	//m_distanceMatrix = distanceMatrix;
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
	this->m_iterations = 0;
	irreationSinceLastBest=0;

	m_RestartBestAnt.setAntTourLength(0);

	m_pLocalSearch = new CLocalSearch(m_distanceMatrix);
 
	m_heuristicMatrix= new std::vector<std::vector<double> >(m_noNodes , m_noNodes);
	m_newPheromoneMatrix = new PheroMatrix(m_noNodes,m_rho);


	m_Ants.empty();
	m_Ants.resize(m_noAnts-1);
	for(int i = 0; i < m_Ants.size(); i++)
	{
		CAnt newAnt(m_noNodes);
		m_Ants[i] = newAnt;
	}
	

	m_randomPath.resize(m_noNodes);
	for (size_t i=0; i<m_noNodes; i++)
		m_randomPath[i]=i;
	std::random_shuffle( m_randomPath.begin() , m_randomPath.end() );
	//m_generator((random_dev());
	t_prob.resize(m_noNodes);
	m_strength.resize(m_noNodes+1);
	
	//new
	m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());
	m_WorstAntToDate.setAntTourLength(0);
	m_BestAntToDate.setAntTourLength((std::numeric_limits<int>::max)());

	calculateNearestNeigbhor();


	
	clog << &std::string("test data\n");
	clog.logMatrix(m_distanceMatrix);

	clog.logMatrix(m_heuristicMatrix);

	clog.logMatrix(m_newPheromoneMatrix->getMatrix());

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
	for(size_t city = 0; city < pant.getNoNodes()-1; city++)
	{
	      
		size_t from = m_Ants[antPos].getCity(city);
		size_t to   = m_Ants[antPos].getCity(city+1);
		// eq 14.2 / 14.3
		m_newPheromoneMatrix->add(from , to , tour_length );	
	}

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
void CAntSystem::decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh)
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
	
	double x = rndSelTrsh(g_rndTravel) * m_strength[m_noNodes];
//	double x = fRand(0,  m_strength[m_noNodes]);
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
	uniform_real<double> rndSelTrsh(0.0,1.0);

	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_Ants.size(); k++)
			decisionRule(k,step, rndSelTrsh);
	}
    //add in the last city to the first city tour ...  
	for(size_t k = 0; k < m_Ants.size(); k++)
	{
			
		size_t tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
		m_Ants[k].setAntTourLength((int)this->calculateAntPathLength(m_Ants[k]));
	}
	updateBestSoFarPath();
}
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
double CAntSystem::calculatePathLength2(const std::vector<size_t> &currentPath) const
{
	size_t  start, end;
	double d = 0;
	for ( size_t i=1; i<currentPath.size(); i++)
	{
		start = currentPath[i-1];
		end = currentPath[i];
		d += (*m_distanceMatrix)[start][end];	
	}
//	d += (*m_distanceMatrix)[currentPath[m_noNodes-1]][currentPath[0]];
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
void CAntSystem::calculateNearestNeigbhor(unsigned int NUMOFANTS)
{
	if(NUMOFANTS >= m_noNodes)
		NUMOFANTS= m_noNodes -1;
	
	unsigned int i = 0;
	int *pHelpArray = new int[m_noNodes];
	double *pdistanceArray = new double[m_noNodes ];
	*(pdistanceArray) =(std::numeric_limits<double>::max)();

	m_nnList.resize(m_noNodes);
	for (unsigned int i = 0; i < m_noNodes; ++i)
		m_nnList[i].resize(NUMOFANTS);


	for (unsigned int node = 0; node < m_noNodes; node++) 
	{ 

		for (i = 0; i < m_noNodes; i++) 
		{
			*(pdistanceArray+i)=(*m_distanceMatrix)[node][i];
			*(pHelpArray+i)=  i;
		}
		double max = (std::numeric_limits<double>::max)() - 1; 
		*(pdistanceArray+node) =  (std::numeric_limits<double>::max)();  // set to a large value .. 
		this->m_pLocalSearch->sort2(pdistanceArray, pHelpArray, 0, static_cast<unsigned int>(m_noNodes - 1));
		for (i = 0; i < NUMOFANTS; i++) 
		{
			m_nnList[node][i] = *(pHelpArray+i);
		}
	}
	delete [] pHelpArray;
	delete [] pdistanceArray;
}
bool CAntSystem::updateBestSoFarPath()
{

	//find the worst ant , best ant , and
	bool bFoundUpdate =false;
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
		bFoundUpdate = true;

	}

	if(m_pWorstAntItt->getAntTourLength() > m_WorstAntToDate.getAntTourLength())
	{
		m_WorstAntToDate = *m_pWorstAntItt;
		std::string s =format("Worst Ant to date found  %f  @ %d",m_WorstAntToDate.getAntTourLength() ,m_iterations);
		static string c; c=s;
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		bFoundUpdate=true;
	}
	if(m_pBestAntItt->getAntTourLength() < m_BestAntToDate.getAntTourLength())
	{
		m_BestAntToDate = *m_pBestAntItt;
		irreationSinceLastBest=0;
		bFoundUpdate =true;
	}
	else
	{
		irreationSinceLastBest++;
	}

	return bFoundUpdate;
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
	
	clog.logMatrix(m_distanceMatrix);

	clog.logMatrix(m_heuristicMatrix);

	clog.logMatrix(m_newPheromoneMatrix->getMatrix());
	
	delete m_heuristicMatrix;
	delete m_pLocalSearch;
	delete m_newPheromoneMatrix;
}
