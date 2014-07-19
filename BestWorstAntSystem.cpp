#include "StdAfx.h"
#include "BestWorstAntSystem.h"
#include "resource.h"
#include <limits>       // std::numeric_limits
#include "Utils.h"



//************************************
// Method:    CBestWorstAntSystem
// FullName:  CBestWorstAntSystem::CBestWorstAntSystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , distanceMatrix)
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//************************************
CBestWorstAntSystem::CBestWorstAntSystem(Parameters& Par,MatrixArrayTypeInt *distanceMatrix) 
	: CAntSystem(Par , distanceMatrix)
{
	Restart =0;
	restart_iteration =1;
	starttime=GetTickCount();
	std::vector<size_t>  randomPath = m_pLocalSearch->greedyPath(m_noNodes);
	m_BestAntToDate.setAntsTour(randomPath);
	if(m_LocalSearchOpt2)
		m_pLocalSearch->opt2(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchOpt3)
		m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchGreed)
		m_pLocalSearch->three_opt_first(m_BestAntToDate.getAntsCityTour() , m_nnList);

	m_BestAntToDate.setAntTourLength(this->calculateAntPathLength(m_BestAntToDate));
	trail_0 = 1. / (m_BestAntToDate.getAntTourLength());
}
/*    
      FUNCTION:      uses additional evaporation on the arcs of iteration worst
                     ant that are not shared with the global best ant
      INPUT:         pointer to the worst (a1) and the best (a2) ant
      OUTPUT:        none
      (SIDE)EFFECTS: pheromones on some arcs undergo additional evaporation
*/
void CBestWorstAntSystem::bwas_worst_ant_update( const CAnt &worseA, const CAnt &bestA)
{  
    size_t   i, pos, pred;
    unsigned int   distance;
    
	std::vector<size_t> pos2(m_noNodes);

    for ( i = 0 ; i < bestA.getNoNodes() ; i++ ) 
	{
		pos2[bestA.getCity(i)] = i;
    }
 
    distance = 0;
    for ( i = 1 ; i < worseA.getNoNodes()   ; i++ ) 
	{
		size_t to = worseA.getCity(i);
		size_t from = worseA.getCity(i-1);
		pos = pos2[to];

		if(pos+1 > m_noNodes)
			pred = m_noNodes - 1;
		else if(pos-1 < 0)
			pred = 0;
		else
			pred = pos;



		size_t bestnextpos = bestA.getCity(pred+1);
		size_t bestprevpos = bestA.getCity(pred-1);

		if (bestnextpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else if (bestprevpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else 
		{   /* edge (j,h) does not occur in ant a2 */       
			m_newPheromoneMatrix->evaporate1(to,from);
			m_newPheromoneMatrix->evaporate1(from,to);
			//(*m_pheromoneMatrix)[j][h] = (1 - m_rho) * (*m_pheromoneMatrix)[j][h];
			//(*m_pheromoneMatrix)[h][j] = (1 - m_rho) * (*m_pheromoneMatrix)[h][j];
		}
    }

}
long CBestWorstAntSystem::distance_between_ants( const CAnt &worseA, const CAnt &bestA)
{
	size_t   i, pos, pred;
	unsigned int   distance=0;

	std::vector<size_t> pos2(m_noNodes+1);
	for ( i = 0 ; i < bestA.getNoNodes() ; i++ ) 
	{
		pos2[bestA.getCity(i)] = i;
	}
	for ( i = 1 ; i < bestA.getNoNodes()   ; i++ ) 
	{
		size_t to = worseA.getCity(i);
		size_t from = worseA.getCity(i-1);
		pos = pos2[to];

		if(pos+1 > m_noNodes)
			pred = m_noNodes - 1;
		else if(pos-1 < 0)
			pred = 0;
		else
			pred = pos;

		size_t bestnextpos = bestA.getCity(pred+1);
		size_t bestprevpos = bestA.getCity(pred-1);

		if (bestnextpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else if (bestprevpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else 
		{   /* edge (j,h) does not occur in ant a2 */       
			distance++;

			//(*m_pheromoneMatrix)[j][h] = (1 - m_rho) * (*m_pheromoneMatrix)[j][h];
			//(*m_pheromoneMatrix)[h][j] = (1 - m_rho) * (*m_pheromoneMatrix)[h][j];
		}
	}
	return distance;
}

//************************************
// Method:    initPheromones
// FullName:  CBestWorstAntSystem::initPheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CBestWorstAntSystem::initPheromones() 
{
	initPheromoneTrails(trail_0);
	//CAntSystem::initPheromones();
}

//************************************
// Method:    initPheromoneTrails
// FullName:  CBestWorstAntSystem::initPheromoneTrails
// Access:    public 
// Returns:   void
// Qualifier: const
// Parameter: double initialValue
//************************************
void CBestWorstAntSystem::initPheromoneTrails(double initialValue) const
{
	std::string s =format("init pheromone trails %.15f  @ %d",initialValue ,m_iterations);
	static string c;
	c=s;
	AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
	
	size_t i, j;
	for (i = 0; i < m_noNodes; i++)
	{
		for (j = 0; j <= i; j++) 
		{
			m_newPheromoneMatrix->set(i , j , initialValue);
		}
	}

	for(i = 0; i < m_noNodes; i++)
		m_newPheromoneMatrix->set(i , i , 0);
}


//The pheromone update is a little bit different: on each iteration, the best to date ant deposits an additional quantity of pheromone on paths it traveled:
//************************************
// Method:    updatePheromones
// FullName:  CBestWorstAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CBestWorstAntSystem::updatePheromones()
{
	//get the best at so far
	this->evaporateAllPhero();


	//global_update_pheromone( best_so_far_ant );
	double d_tau = (double)1.0/ m_BestAntToDate.getAntTourLength(); 
	for(size_t city = 1; city <m_BestAntToDate.getNoNodes(); city++)
	{
		size_t from = m_BestAntToDate.getCity(city-1);
		size_t to = m_BestAntToDate.getCity(city);
		this->m_newPheromoneMatrix->add(from,to , d_tau);
	}

	bwas_worst_ant_update(*m_pWorstAntItt , m_BestAntToDate );
	long distance_best_worst = distance_between_ants(m_BestAntToDate,*m_pWorstAntItt );
	if ( distance_best_worst < (int) (0.05 * (double) m_noNodes) ) 
	{
	//	m_RestartBestAnt.setAntTourLength((std::numeric_limits<int>::max)());
		initPheromoneTrails(trail_0);//this->m_newPheromoneMatrix->averagePheromoneTrail(m_RestartBestAnt.getAntsCityTour()));
		//CAntSystem::initPheromones();
		restart_iteration = m_iterations;
		restart_time = GetTickCount();
	}
	else 
	{
		bwas_pheromone_mutation();
	}

	calculateHeuristicMatrix();
 }
void CBestWorstAntSystem::bwas_pheromone_mutation( void )
{

	unsigned int     i, j, k;
	unsigned int     num_mutations;
	double       avg_trail = 0.0,  mutation_rate = 0.3;

     /* compute average pheromone trail on edges of global best solution */
	avg_trail = this->m_newPheromoneMatrix->averagePheromoneTrail(m_BestAntToDate.getAntsCityTour());

		
	double mutation_strength = 4.0 * (double)avg_trail * (double) (GetTickCount() - restart_time) / (300000- (restart_time + 0.0001));
	

	mutation_rate = mutation_rate / this->m_noNodes *m_noAnts;
	num_mutations = (int) (this->m_noNodes * mutation_rate) / 2;
		
	

    /* / 2 because of adjustment for symmetry of pheromone trails */
 
    if ( restart_iteration < 2 )
	     num_mutations = 0; 

    for ( i = 0 ; i < num_mutations ; i++ ) 
	{
		j =   (int) (fRand(0, (unsigned int)m_newPheromoneMatrix->size()-1  ));
		k =   (int) (fRand(0, (unsigned int)m_newPheromoneMatrix->size()-1  ));
		double chance =  fRand(0,1);
		if ( chance < 0.5 ) 
		{
			this->m_newPheromoneMatrix->add(j,k , mutation_strength);
		}
		else 
		{	
			this->m_newPheromoneMatrix->subtract(j,k , mutation_strength);

		}
    }
}




CBestWorstAntSystem::~CBestWorstAntSystem(void)
{
}
