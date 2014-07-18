#include "StdAfx.h"
#include "CElitistAntSystem.h"

/*
A first improvement on the initial AS, called the elitist strategy for Ant System
(EAS), was introduced in Dorigo (1992) and Dorigo et al., (1991a, 1996). The idea is
to provide strong additional reinforcement to the arcs belonging to the best tour
found since the start of the algorithm; this tour is denoted as Tbs (best-so-far tour) in
the following. Note that this additional feedback to the best-so-far tour (which can
be viewed as additional pheromone deposited by an additional ant called best-so-far
ant) is another example of a daemon action of the ACO metaheuristic.
/*/

//************************************
// Method:    CElitistAntSystem
// FullName:  CElitistAntSystem::CElitistAntSystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , distanceMatrix)
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//get the best distance and adjust phero with that distance
//reported, that the best value for mWeight is between four and six.
//************************************
CElitistAntSystem::CElitistAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix) 
	: CAntSystem(Par , distanceMatrix) , mWeight( Par.EliteWeight)
{
}
//
//************************************
// Method:    updatePheromones
// FullName:  CElitistAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
// The pheromone update is a little bit different: on each iteration,
//the best to date ant deposits an additional quantity of pheromone on paths it traveled:
//************************************
void CElitistAntSystem::updatePheromones()
{
	evaporateAllPhero();
	double d_tau = (double)mWeight/ m_BestAntToDate.getAntTourLength(); 
	for(size_t city = 1; city <m_BestAntToDate.getNoNodes(); city++)
	{
		size_t from = m_BestAntToDate.getCity(city-1);
		size_t to = m_BestAntToDate.getCity(city);
 		// eq 14.2 / 14.3
		this->m_newPheromoneMatrix->add(from , to , d_tau);	
	}
	for(unsigned int k = 0; k <m_Ants.size(); k++)
		adjustPheremone(k);
	calculateHeuristicMatrix();
}

//************************************
// Method:    ~CElitistAntSystem
// FullName:  CElitistAntSystem::~CElitistAntSystem
// Access:    virtual public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
CElitistAntSystem::~CElitistAntSystem(void)
{
}
