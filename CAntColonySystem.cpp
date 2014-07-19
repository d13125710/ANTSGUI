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
//	delete m_pOpt;
}
void CAntColonySystem::evaporateAllPhero()
{ 
//	CAnt ant = this->getBestAntToDate();

	for (unsigned int i=1; i < m_BestAntToDate.getNoNodes(); i++) 
	{
		size_t idx1 = m_BestAntToDate.getCity(i); //h
		size_t idx2 = m_BestAntToDate.getCity(i-1); //j
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
	
	double delta = 1.0 / (m_BestAntToDate.getAntTourLength());
	//dont need last city as added in this tour
	for(size_t i = 1; i < m_BestAntToDate.getNoNodes(); i++)
	{ 
		size_t idx1 = m_BestAntToDate.getCity(i); //h
		size_t idx2 = m_BestAntToDate.getCity(i-1); //j
		//localPheromoneEndIndexUpdate(idx1 ,idx2);
		 this->m_newPheromoneMatrix->add(idx1,idx2 ,(m_r0*delta));
	//	pheromone[idx1][idx2]+=ro*delta;
	//	pheromone[idx2][idx1]+=ro*delta;



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

bool CAntColonySystem::updateBestSoFarPath()
{
	return CAntSystem::updateBestSoFarPath();

}
//no need
void CAntColonySystem::calculateHeuristicMatrix()
{
   // CAntSystem::calculateHeuristicMatrix();

	for(size_t i = 0; i < m_noNodes; i++)
		for(size_t j = 0; j <m_noNodes; j++)
		{
			CalculateHeuristicForNode(i, j);
		}	
}
void CAntColonySystem::updatePheromones()
{ 
	
//	evaporateAllPhero(); //this
	//this->m_newPheromoneMatrix->evaporate_all();
	globalPheromoneDeposit();
	calculateHeuristicMatrix();
}



void CAntColonySystem::initPheromones()
{

	double best_distance = (std::numeric_limits<double>::max)();
	for(int i =0 ; i < m_noNodes; i++)
	{
		
		std::vector<size_t> randomPath(m_noNodes);
		for (size_t i=0; i<m_noNodes; i++)
			randomPath[i]=i;
		
		std::random_shuffle( randomPath.begin() , randomPath.end());
		m_pLocalSearch->greedyAntPath(randomPath);
			m_pLocalSearch->opt3(randomPath);
		double dist=this->calculatePathLength(randomPath);
	
		if(dist < best_distance  )
		{
		
			m_BestAntToDate.setAntsTour(randomPath);
			m_BestAntToDate.setAntTourLength(calculatePathLength(randomPath));
			best_distance = dist;
		}
	}
	m_BestAntToDate.getAntsCityTour().resize(m_noNodes+1);
	size_t index = m_BestAntToDate.getCity(0);
	m_BestAntToDate.setAntCity(m_noNodes , index );


	tau0 = 1.0/ ((double)m_noAnts * best_distance);

	for (size_t  i=0; i<m_noNodes; i++)	
	{
		for (size_t j=0; j< m_noNodes; j++)
			this->m_newPheromoneMatrix->set(i, j , tau0);
	}

	for(size_t i = 0; i < m_noNodes; i++)
		m_newPheromoneMatrix->set(i , i , 0);

		
//	m_pLocalSearch->opt3(nntour);
	
//m_bestSoFarPath  = m_pLocalSearch->greedyPath(this->m_noNodes);
//	m_pLocalSearch->opt3(m_bestSoFarPath);

	//m_bestSoFarPathlength=this->calculatePathLength(m_bestSoFarPath);
	//this->m_bestSoFarPathlength = (int)lenght;

	/*
	tau0 = 1/ ((double)m_noNodes * best_distance);

	for (unsigned int i=0; i<m_noNodes; i++)	
	{
		for (unsigned int j=0; j< m_noNodes; j++)
			this->m_newPheromoneMatrix->set(i, j , tau0);
	}
	*/
	//min max

	std::vector<bool> visited(m_noNodes);
	std::vector<size_t> nntour(m_noNodes+1);
	//calculate min max values inital
	int phase = 0;
	int rnd=(rand()%(visited.size()-1))+1;
	nntour[0] =rnd;
	visited[rnd] = true;
	chooseClosestNext(visited, nntour);
	//double distance = this->calculatePathLength( nntour);
		nntour[m_noNodes] = nntour[0];
	//m_pLocalSearch->opt3(nntour);
		//
	m_pLocalSearch->three_opt_first(nntour , m_nnList);
	
	
	double distance = this->calculatePathLength2( nntour);
	tau0 =  1. / ((m_noNodes) * distance);
	//tau0 = 1. / ((m_rho) * distance) / (2. * m_noAnts/m_noNodes);
	
	unsigned int i, j;

	for (i = 0; i < m_noNodes; i++)
	{
		for (j = 0; j <= i; j++) 
		{
			m_newPheromoneMatrix->set(i , j , tau0);
		}
	}

	m_BestAntToDate.setAntsTour(nntour);
	m_BestAntToDate.setAntTourLength(distance);


	for(i = 0; i < m_noNodes; i++)
		m_newPheromoneMatrix->set(i , i , 0);
	for(unsigned int i=0;i<this->m_noNodes;i++)
		this->m_newPheromoneMatrix->set(i, i , 0.0);

	calculateHeuristicMatrix();

//	this->updateBestSoFarPath();

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
		
		//double x = fRand(0,  m_strength[m_noNodes]);
		
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
	//	x = fRand(0, 1);	
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
			//decisionRule2(k,step);
			localPheromoneUpdate(k,step);
		
		}
	}
     
	for(int k = 0; k <  m_Ants.size(); k++)
	{
		int tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
	
		//const std::vector<size_t>& tourvector = m_Ants[k].getAntsCityTour();
		//m_Ants[k].setAntTourLength((int)calculatePathLength(tourvector));
		//update the ant
		m_Ants[k].setAntTourLength((int)this->calculateAntPathLength(m_Ants[k]));


	
		size_t idx1 = this->m_Ants[k].getCity(m_noNodes);
		size_t idx2 = m_Ants[k].getCity(m_noNodes-1);
		localPheromoneEndIndexUpdate(idx1, idx2);

		
		//double dist = (*m_distanceMatrix)[idx1][idx2];

		//double xx = m_newPheromoneMatrix->get(i,j);
		//double yy = (*m_distanceMatrix)[idx1][idx2];
		
		//(*m_heuristicMatrix)[i][j] =(double) (yy) ?  abs(pow( xx , m_alpha) * pow(yy,m_beta)) :  abs(pow( xx , m_alpha) * pow(1,m_beta));


	//	(*m_heuristicMatrix)[idx1][idx2] = pow( updateValue,m_alpha) * pow(HEURISTIC(idx1,idx2),m_beta);
	//	(*m_heuristicMatrix)[idx2][idx1] = (*m_heuristicMatrix)[idx1][idx2];



		//localPheromoneUpdate(k,m_noNodes-1);
	}
	updateBestSoFarPath();
}

     void CAntColonySystem::choose_closest_next(CAnt& a, size_t phase)
    /*
     * FUNCTION: Chooses for an ant the closest city as the next one
     * INPUT: pointer to ant and the construction step "phase"
     * OUTPUT: none
     * (SIDE)EFFECT: ant moves to the chosen city
     */
    {
	size_t city, current_city, next_city; int min_distance;
		std::vector<size_t> &tour = a.getAntsCityTour();

		next_city = m_noNodes;
	
		current_city = tour[phase - 1];
		min_distance = (std::numeric_limits<int>::max)();
		for (city = 0; city < m_noNodes; city++) {
			if (a.isCityVisited(city))
			; /* city already visited */
			else {
			if ((*m_distanceMatrix)[current_city][city] < min_distance) {
				next_city = city;
				min_distance = (*m_distanceMatrix)[current_city][city];
			}
			}
		}
	
		a.setAntCity(phase , next_city);
		a.setCityVisited(next_city);
    }


void CAntColonySystem::decisionRule2(size_t k, size_t phase)
{
	//roulette wheel selection  
	size_t help;
	size_t current_city;
	double partial_sum = 0., sum_prob = 0.0;

	CAnt& a = m_Ants[k]; 


	double x=fRand(0, 1);	
	if(x < q0)  
	{
		neighbour_choose_best_next(m_Ants[k], phase);
		
	}
	else //if exploitation
	{
		std::vector<size_t> &tour = a.getAntsCityTour();

		current_city = tour[phase - 1]; /* current_city city of ant k */
		assert (current_city >= 0 && current_city < m_noNodes);
		for (int i = 0; i < m_nnList[current_city].size(); i++) 
		{
				if (a.isCityVisited(m_nnList[current_city][i]))
				{
						this->t_prob[i] = 0.0; /* city already visited */
						m_strength[i] = 0; 	
				}
				else 
				{
					//assert (Tsp.instance.nn_list[current_city][i] >= 0 && Tsp.instance.nn_list[current_city][i] < Tsp.n);
					t_prob[i] = (*m_heuristicMatrix)[current_city][m_nnList[current_city][i]];
					sum_prob += t_prob[i];
				}
		}

		if (sum_prob <= 0.0) 
		{
			/* All cities from the candidate set are tabu */
			choose_best_next(a, phase);
		} 
		else 
		{
			/*
			* at least one neighbor is eligible, chose one according to the
			* selection probabilities
			*/
			for (unsigned int z =0; z < m_noNodes; z++)
				m_strength[z+1] = t_prob[z] + m_strength[z];

			double x = fRand(0,  m_strength[m_noNodes]);

			unsigned int j = 0;
			while (!((m_strength[j] <= x) && (x <= m_strength[j+1])))
				j++;


			//rnd  = fRand(0,  sum_prob);;
			//int i = 0;
			//partial_sum = t_prob[i];
			/* This loop always stops because prob_ptr[nn_ants] == HUGE_VAL */
			//while (partial_sum < rnd) 
			//{
			//	i++;
			//	partial_sum += t_prob[i];
			//}
			/*
			* This may very rarely happen because of rounding if rnd is
			* close to 1.
			*/
			if (j == m_noAnts) 
			{
				neighbour_choose_best_next(a, phase);
				return;
			}
	
			help = m_nnList[current_city][j];
	
			a.setAntCity(phase , help);
			a.setCityVisited(help);
		}
	}

}

void CAntColonySystem::chooseClosestNext(std::vector<bool> &antsvisted , std::vector<size_t> &nntour)
{
	size_t city, current_city, next_city;
	int min_distance;
	next_city =this->m_noNodes;

	for(size_t phase = 1; phase < m_noNodes; phase++)
	{
		current_city = nntour[phase - 1];
		min_distance =(std::numeric_limits<int>::max)();

		for (city = 0; city < m_noNodes; city++)
		{
			if (antsvisted[city])
				; /* city already visited */
			else
			{

				if ((*m_distanceMatrix)[current_city][city] < min_distance)
				{
					next_city = city;
					min_distance = (*m_distanceMatrix)[current_city][city];
				}
			}
		}


		nntour[phase] = next_city;
		antsvisted[next_city] = true;
	}

}

void CAntColonySystem::choose_best_next(CAnt& a, size_t phase)
    /*
     * FUNCTION: chooses for an ant as the next city the one with
     * maximal value of heuristic information times pheromone
     * INPUT: pointer to ant and the construction step
     * OUTPUT: none
     * (SIDE)EFFECT: ant moves to the chosen city
     */
    {
	size_t city, current_city, next_city;
	double value_best;

	next_city = m_noNodes;
	assert (phase > 0 && phase < m_noNodes);

	std::vector<size_t> &tour = a.getAntsCityTour();
	current_city = tour[phase - 1];

	value_best = -1.; /* values in total matrix are always >= 0.0 */
	for (city = 0; city < m_noNodes; city++) {
	    if (a.isCityVisited(city))
		; /* city already visited, do nothing */
	    else {
		if ((*m_heuristicMatrix)[current_city][city] > value_best) {
		    next_city = city;
		    value_best = (*m_heuristicMatrix)[current_city][city];
		}
	    }
	}
	a.setAntCity(phase , next_city);
	a.setCityVisited(next_city);// = true;
    }

 void CAntColonySystem::neighbour_choose_best_next(CAnt &a, size_t phase)
    /*
     * FUNCTION: chooses for an ant as the next city the one with
     * maximal value of heuristic information times pheromone
     * INPUT: pointer to ant and the construction step "phase"
     * OUTPUT: none
     * (SIDE)EFFECT: ant moves to the chosen city
     */
    {
	size_t i, current_city, next_city, help_city;
	double value_best, help;
	std::vector<size_t> &tour = a.getAntsCityTour();

	next_city = m_noNodes;
	
	current_city = a.getAntsCityTour()[phase - 1];
	
	value_best = -1.; /* values in total matix are always >= 0.0 */
	for (i = 0; i < m_nnList[current_city].size(); i++) 
	{
	    help_city = this->m_nnList[current_city][i];
	    if (a.isCityVisited(help_city))
		; /* city already visited, do nothing */
	    else 
		{
			help = (*m_heuristicMatrix)[current_city][help_city];
			if (help > value_best) {
				value_best = help;
				next_city = help_city;
			}
	    }
	}
	if (next_city == m_noNodes)
	    /* all cities in nearest neighbor list were already visited */
	    choose_best_next(a, phase);
	else
	{
		a.setAntCity(phase , next_city);
	    a.setCityVisited(next_city);
	}
}

