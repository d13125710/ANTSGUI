#include "StdAfx.h"
#include "CElitistAntSystem.h"


//get the best distance and adjust phero with that distance
//reported, that the best value for mWeight is between four and six.
CElitistAntSystem::CElitistAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix) 
	: CAntSystem(Par , distanceMatrix)
{
	this->mWeight = Par.EliteWeight; 
}
//The pheromone update is a little bit different: on each iteration, the best to date ant deposits an additional quantity of pheromone on paths it traveled:
void CElitistAntSystem::updatePheromones()
{
	evaporateAllPhero();
	//add best tour stuff
	double d_tau = (double)mWeight/ m_bestSoFarPathlength; 
	for(size_t city = 1; city <m_bestSoFarPath.size(); city++)
	{
		size_t from = m_bestSoFarPath[city-1];
		size_t to = m_bestSoFarPath[city];
 		// eq 14.2 / 14.3
		this->m_newPheromoneMatrix->add(from , to , d_tau);	
	}
	for(unsigned int k = 0; k < m_noNodes; k++)
		adjustPheremone(k);
	calculateHeuristicMatrix();
}

CElitistAntSystem::~CElitistAntSystem(void)
{
}
