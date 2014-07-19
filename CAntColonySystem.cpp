#include "StdAfx.h"
#include "CAntColonySystem.h"

#include <iostream>
#include <assert.h>  
#include <limits>       // std::numeric_limits



CAntColonySystem::CAntColonySystem(Parameters &Par, MatrixArrayTypeInt *matrix)
			: CAntSystem(Par , matrix) 
{
	this->xi = Par.xi;
    this->q0 =Par.q0;
	this->m_r0 = Par.r0;
    this->tau0 = 0;
	this->m_noAnts=25;
	if(m_noNodes <= m_noAnts )
		m_noAnts = 30;

	//this->m_rho=0.1;
	//uniform_real<double> m_ChoiceExplore(0.0, 1);

}

CAntColonySystem::~CAntColonySystem(void)
{

}
void CAntColonySystem::evaporateAllPhero()
{ 

	for (size_t i=1; i < m_BestAntToDate.getNoNodes(); i++) 
	{
		size_t idx1 = m_BestAntToDate.getCity(i); 
		size_t idx2 = m_BestAntToDate.getCity(i-1); 
		m_newPheromoneMatrix->multipy(idx1 , idx2 ,(1-m_r0));
	}

}
/*
In ACS only the globally best ant (i.e., the ant which constructed the shortest tour from the
beginning of the trial) is allowed to deposit pheromone. This choice, together with the use of
the pseudo-random-proportional rule, is intended to make the search more directed: Ants
search in a neighborhood of the best tour found up to the current iteration of the algorithm.
Global updating is performed after all ants have completed their tours. The pheromone level
is updated by applying the global updating rule of Eq. (4)
*/


 //************************************
 // Method:    globalPheromoneDeposit
 // FullName:  CAntColonySystem::globalPheromoneDeposit
 // Access:    public 
 // Returns:   void
 // Qualifier:
 //************************************
 void CAntColonySystem::globalPheromoneDeposit()
{
	
	double delta = 1.0 / ((double)(m_BestAntToDate.getAntTourLength()));
	for(size_t i = 1; i < m_BestAntToDate.getNoNodes(); i++)
	{ 
		size_t idx1 = m_BestAntToDate.getCity(i);
		size_t idx2 = m_BestAntToDate.getCity(i-1); 
	    this->m_newPheromoneMatrix->add(idx1,idx2 ,(m_r0*delta));
	}
	
}
 //************************************
 // Method:    CalculateHeuristicForNode
 // FullName:  CAntColonySystem::CalculateHeuristicForNode
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t i
 // Parameter: size_t j
 //************************************
 void CAntColonySystem::CalculateHeuristicForNode(size_t i, size_t j)
 {
	 double xx = m_newPheromoneMatrix->get(i,j);
	 double yy = (*m_distanceMatrix)[i][j];
	 (*m_heuristicMatrix)[i][j] = abs(pow( xx , m_alpha) * pow(HEURISTIC(i,j),m_beta));
	 (*m_heuristicMatrix)[j][i] =  (*m_heuristicMatrix)[i][j] ;
 }
 //************************************
 // Method:    localPheromoneEndIndexUpdate
 // FullName:  CAntColonySystem::localPheromoneEndIndexUpdate
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t idx1
 // Parameter: size_t idx2
 //************************************
 void CAntColonySystem::localPheromoneEndIndexUpdate(size_t idx1, size_t idx2)
 {
	 double currentValue = this->m_newPheromoneMatrix->get(idx1, idx2);
	 double updateValue =  (1.0-xi)*currentValue+xi*tau0;
	 this->m_newPheromoneMatrix->set(idx1, idx2 , updateValue);
	 CalculateHeuristicForNode(idx1, idx2);
 }
 //************************************
 // Method:    localPheromoneUpdate
 // FullName:  CAntColonySystem::localPheromoneUpdate
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t ant
 // Parameter: size_t step
 //************************************
 void CAntColonySystem::localPheromoneUpdate(size_t ant, size_t step)
 {
	 size_t idx1 = this->m_Ants[ant].getCity(step);
	 size_t idx2 = m_Ants[ant].getCity(step-1);
	 localPheromoneEndIndexUpdate(idx1, idx2);
 } 

//************************************
// Method:    calculateHeuristicMatrix
// FullName:  CAntColonySystem::calculateHeuristicMatrix
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::calculateHeuristicMatrix()
{
	for(size_t i = 0; i < m_noNodes; i++)
		for(size_t j = 0; j <m_noNodes; j++)
		{
			CalculateHeuristicForNode(i, j);
		}	
}
//************************************
// Method:    updatePheromones
// FullName:  CAntColonySystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::updatePheromones()
{ 
	
//	evaporateAllPhero(); //this
	//this->m_newPheromoneMatrix->evaporate_all();
	globalPheromoneDeposit();
	calculateHeuristicMatrix();
}
//************************************
// Method:    initPheromones
// FullName:  CAntColonySystem::initPheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::initPheromones()
{
	double best_distance = (std::numeric_limits<double>::max)();
	std::vector<size_t> randomPath = m_pLocalSearch->greedyPath(m_noNodes);
	double dist=this->calculatePathLength(randomPath);
	m_BestAntToDate.setAntsTour(randomPath);
	if(m_LocalSearchOpt2)
		m_pLocalSearch->opt2(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchOpt3)
		m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchGreed)
		m_pLocalSearch->three_opt_first(m_BestAntToDate.getAntsCityTour() , m_nnList);
	
	m_BestAntToDate.setAntTourLength(this->calculateAntPathLength(m_BestAntToDate));
	tau0 = 1.0/ ((double)m_noNodes * dist);
	
	for (size_t  i=0; i<m_noNodes; i++)	
	{
		for (size_t j=0; j< m_noNodes; j++)
			this->m_newPheromoneMatrix->set(i, j , tau0);
	}

	for(size_t i = 0; i < m_noNodes; i++)
		m_newPheromoneMatrix->set(i , i , 0);

	calculateHeuristicMatrix();
}
/*
The procedure works as follows: first, the current city c of ant k is determined
(line 1). The probabilistic choice of the next city then works analogously
to the roulette wheel selection procedure of evolutionary computation (Goldberg,
1989): each value choice_info½c½ j of a city j that ant k has not visited yet determines
a slice on a circular roulette wheel, the size of the slice being proportional to the
weight of the associated choice (lines 2–10). Next, the wheel is spun and the city to
which the marker points is chosen as the next city for ant k (lines 11–17). This is
implemented by
1. summing the weight of the various choices in the variable sum_probabilities,
2. drawing a uniformly distributed random number r from the interval ½0; sum_
probabilities,
3. going through the feasible choices until the sum is greater or equal to r.
Finally, the ant is moved to the chosen city, which is marked as visited (lines 18
and 19).
*/

void CAntColonySystem::decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh, uniform_real<double> &choiceE)
{
     //roulette wheel selection  
	size_t c = m_Ants[k].getCity(step-1); 
	double sum_prob =0;
 
		for (size_t i=0; i<m_noNodes; i++) 
		{
			t_prob[i] = 0.0;
			m_strength[i] = 0; 	
			if (m_Ants[k].isCityVisited(i) == false && i != c)	
			{
				t_prob[i]= (*m_heuristicMatrix)[c][i];
				sum_prob +=t_prob[i];
			}
		}
		for (size_t z =0; z < m_noNodes; z++)
			m_strength[z+1] = t_prob[z] + m_strength[z];
				
		//rndSelTrsh
		double x = rndSelTrsh(g_rndTravel) * m_strength[m_noNodes];
		
		int j = 0;
		while (!((m_strength[j] <= x) && (x <= m_strength[j+1])))
		  j++;
		
		size_t randomDecision =j;

		//find best neg
		double maxHeuristic = -1;
        int maxHeuristicIdx = -1;
        for(j = 0; j < m_noNodes; j++)
		{
			 double choice = (*m_heuristicMatrix)[c][j];
			 if(maxHeuristic < choice && !(m_Ants[k].isCityVisited(j))) //&& c!=j)
			 {
				maxHeuristic = choice;
				maxHeuristicIdx = j;
			}
        }
		x=choiceE(g_rndTravel);

        if(x < q0)  
		{
            m_Ants[k].setAntCity(step, maxHeuristicIdx);
            m_Ants[k].setCityVisited(maxHeuristicIdx);
        }
        else //if exploitation
		{
			//	std::cout << maxHeuristicIdx << ",";
            m_Ants[k].setAntCity(step, randomDecision);
            m_Ants[k].setCityVisited(randomDecision);
        }

}
/*
1. First, the ants’ memory must be emptied. This is done in lines 1 of procedure
ConstructSolutions by marking all cities as unvisited, that is, by setting all the entries
of the array ants.visited to false for all the ants.
2. Second, each ant has to be assigned an initial city. One possibility is to assign each
ant a random initial city. This is accomplished in lines 6 to 11 of the procedure. The
function random returns a random number chosen according to a uniform distribution
over the set f1; . . . ; ng.
3. Next, each ant constructs a complete tour. At each construction step (see the procedure
in figure 3.17) the ants apply the AS action choice rule [equation (3.2)]. The
procedure ASDecisionRule implements the action choice rule and takes as parameters
the ant identifier and the current construction step index; this is discussed below in
more detail.
4. Finally, in lines 18 to 21, the ants move back to the initial city and the tour length
of each ant’s tour is computed. Remember that, for the sake of simplicity, in the tour
representation we repeat the identifier of the first city at position n þ 1; this is done in
line 19.
*/
void CAntColonySystem::constructSolutions()
{
	//clear ants memory and shuffle start pos
	initAnts();
	//place ants in ramdom citys for starting
	for (int k = 0; k <  m_Ants.size(); k++ )
	{
		m_Ants[k].setAntCity(0,m_randomPath[k]);
		m_Ants[k].setCityVisited(m_randomPath[k]);
	}
	uniform_real<double> rndSelTrsh(0.0, 1);
	uniform_real<double> m_ChoiceExplore(0.0 , 1.0);

	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_Ants.size(); k++)
		{
			decisionRule(k,step, rndSelTrsh, m_ChoiceExplore);
			localPheromoneUpdate(k,step);
		}
	}
     
	for(int k = 0; k <  m_Ants.size(); k++)
	{
		int tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
			
		m_Ants[k].setAntTourLength((int)this->calculateAntPathLength(m_Ants[k]));

		size_t idx1 = this->m_Ants[k].getCity(m_noNodes);
		size_t idx2 = m_Ants[k].getCity(m_noNodes-1);
		localPheromoneEndIndexUpdate(idx1, idx2);
	}
	updateBestSoFarPath();
}