#include "StdAfx.h"
#include "CAntSystem.h"
#include "resource.h"

#include "localsearch.h"

//CAntSystem::CAntSystem(int noNodes, int noAnts)
//{
	//Create(noNodes, noAnts);
//}

CAntSystem::CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix)
{
	m_distanceMatrix = distanceMatrix;
	Create(Par);
   
}
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
	worseindex=-1;
	m_pLocalSearch = new CLocalSearch(m_distanceMatrix);
    this->m_iterations = 0;
	//m_doOpt3
	m_heuristicMatrix= new std::vector<std::vector<double> >(m_noNodes , m_noNodes);
	//test
	m_newPheromoneMatrix = new PheroMatrix(m_noNodes,m_rho);

	m_bestSoFarPath.resize(m_noNodes);
	m_worstSoFarPath.resize(m_noNodes);

	m_Ants.empty();
	m_Ants.resize(m_noAnts);
	for(int i = 0; i < m_noAnts; i++)
	{
		CAnt newAnt(m_noNodes);
		m_Ants[i] = newAnt;
	}
	
	m_bestIrrPath.resize(m_noNodes);  //refactor
	m_worstIrrPath.resize(m_noNodes);



	m_randomPath.resize(m_noNodes);
	for (size_t i=0; i<m_noNodes; i++)
		m_randomPath[i]=i;
	std::random_shuffle( m_randomPath.begin() , m_randomPath.end() );
	//m_generator((random_dev());
	t_prob.resize(m_noNodes);
	m_strength.resize(m_noNodes+1);
	
	//set the lenths of the paths
	m_bestSoFarPathlength = (std::numeric_limits<int>::max)();
	m_worstSoFarPathlength =0;

	m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());

}

void  CAntSystem::updatePheromones()
{
    evaporateAllPhero();
    for(size_t k = 0; k < m_noNodes; k++)
        adjustPheremone(k);
	//dont think i need this on irreration
    calculateHeuristicMatrix();
}

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
	CAnt &pant=m_Ants[antPos];
	double tour_length = 1 / calculatePathLength(pant.getAntsCityTour()); 
	for(size_t city = 1; city < m_noNodes-1; city++)
	 {
	      
		size_t from = m_Ants[antPos].getCity(city);
		size_t to   = m_Ants[antPos].getCity(city+1);
		// eq 14.2 / 14.3
		m_newPheromoneMatrix->add(from , to , tour_length );	
  }
	size_t from = m_Ants[antPos].getCity(m_noNodes-1);
	size_t to   = m_Ants[antPos].getCity(0);
	// eq 14.2 / 14.3
	m_newPheromoneMatrix->add(from , to , tour_length );	

}

void CAntSystem::decisionRule(size_t k, size_t step)
{
        size_t c = m_Ants[k].getCity(step-1); 
		for (size_t i=0; i<m_noNodes; i++) 
		{
			t_prob[i] = 0.0;
			m_strength[i] = 0;	
			if (m_Ants[k].isCityVisited(i) == false && c!=i)	
			{
				t_prob[i]= (*m_heuristicMatrix)[c][i];
			}
		}
		for (size_t z =0; z < m_noNodes; z++)
			m_strength[z+1] = t_prob[z] + m_strength[z];
		double x = fRand(0,  m_strength[m_noNodes]);
		int y = 0;
		while (!((m_strength[y] <= x) && (x <= m_strength[y+1])))
			y++;
			
		m_Ants[k].setAntCity(step, y);
		m_Ants[k].setCityVisited(y);

}


void CAntSystem::constructSolutions()
{
	initAnts();
	   
	for (size_t k = 0; k < m_noAnts; k++ )
	{
		m_Ants[k].setAntCity(0,m_randomPath[k]);
		m_Ants[k].setCityVisited(m_randomPath[k]);

	}

	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_noAnts; k++)
			decisionRule(k,step);
	}
    //add in the last city to the first city tour ...  
	for(size_t k = 0; k < m_noAnts; k++){
			
	//	int tourstart=m_Ants[k].getCity(0);
	//	m_Ants[k].setAntCity(m_noNodes,tourstart);
		const std::vector<size_t>& tourvector = m_Ants[k].getAntsCityTour();
		m_Ants[k].setAntTourLength((int)calculatePathLength(tourvector));
	}
	updateBestSoFarPath();
}

std::vector<size_t> &CAntSystem::getBestTourIrreration()
{
    int bestTourLength = (std::numeric_limits<int>::max)();
	for(size_t j = 0; j < m_noNodes; j++)
	{
		m_bestIrrPath[j] = m_Ants[BestAntIndex].getCity(j);
	}
    return m_bestIrrPath;
}
std::vector<size_t> &CAntSystem::getWorstTourIrreration()
{
	int worstTourLength = 0;
	for(size_t j = 0; j < m_noNodes; j++)
	{
		m_worstIrrPath[j] = m_Ants[worseindex].getCity(j);
	}
	return m_worstIrrPath;
}


void CAntSystem::evaporateAllPhero()
{
	m_newPheromoneMatrix->evaporate_all();
}

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
void CAntSystem::calculateHeuristicMatrix()
{
       for(size_t i = 0; i < m_noNodes; i++)
          for(size_t j = 0; j <m_noNodes; j++)
		  {
			 double xx = m_newPheromoneMatrix->get(i,j);
			 double yy = (*m_distanceMatrix)[i][j];
			 if(yy==0)
				 yy=1;
			 (*m_heuristicMatrix)[i][j] = pow( xx , m_alpha) / pow(yy,m_beta);
		  }	
}
void CAntSystem::updateBestSoFarPath()
{
	
		double distance=  this->calculatePathLength(m_bestSoFarPath);
		int bestitter = computeAntsItterationBestIndex() ;
		if(distance == 0 || this->m_iterations ==0)
		{
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
		


		




}


CAntSystem::~CAntSystem(void)
{
	delete m_heuristicMatrix;
	delete m_pLocalSearch;
	delete m_newPheromoneMatrix;
}
