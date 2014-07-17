#include "stdafx.h"
#include "GeneticAlgorithm.h"

#include <sstream>

const std::string filepath = "C:\\dump\\best.txt";


GeneticAlgorithm::GeneticAlgorithm(void)
{
	// Give it some default parameters
	mutationRate    = 10;
	crossoverRate   = 10;
	populationSize  = 100;
	numberIterations = 10000;	
	tournamentSize = populationSize / 5;
	bestFitness = infinity;	
	bestIndex = -1;
}


GeneticAlgorithm::~GeneticAlgorithm(void)
{
}


// Initialize parameters, generate population etc
void GeneticAlgorithm::Initialize( const int& crate,
								   const int& mrate,
								   const int& psize,
								   const int& iter, 
								   const int& tsize,								   
								   const std::string& path,
								   CoordMatrix* mat )
{
	pop.SetMatrix( mat );
	pop.SetTourSize( mat->size() );
	TourSize = mat->size();
	SetParameters( crate, mrate, psize, iter, tsize );
	CreatePopulation();
	log.Open( path.c_str() );
}


// Run the genetic algorithm
Tour* GeneticAlgorithm::Run( const int& iter,
							 CoordMatrix* mat )
{
	bestIndex = Evaluate();
	Tour* tour = pop.GetTour( bestIndex );
	LogResult( tour->TourDistance( mat ), iter, 10 );	
	Select();
	Crossover();
	Mutate();
	
	return tour;
}


// Evaulate fitnesses of population Tours
int GeneticAlgorithm::Evaluate()
{
	int best = pop.EvaluatePopulation();		
	Tour* tour = pop.GetTour( best );

	if ( tour->GetFitness() < bestFitness )
	{
		bestFitness = tour->GetFitness();
		bestIndex = best;
	}

	return bestIndex;
}


// Apply crossover to selected Tour pairs
void GeneticAlgorithm::Crossover()
{
	for ( int i = 0; i < populationSize; i++ )
	{
		int r = rand() % populationSize;

		if ( r < crossoverRate && r != bestIndex )
		{
			// Select random pair for crossover
			int index1 = rand() % populationSize;
			int index2 = rand() % populationSize;
			
			while ( index1 == index2 )
			{
				index2 = rand() % populationSize;
			}

			// Get crossover points
			// Point1: 0 - 31
			int point1 = rand() % TourSize / 2;

			// Point1: 32 - 64
			int point2 = TourSize / 2 +
				         rand() % ( TourSize / 2 );

			while ( point1 == point2 )
			{
				point2 = TourSize / 2 +
				         rand() % ( TourSize / 2 );
			}

			if ( point1 > point2 )
			{
				int tmp = point1;
				point1 = point2;
				point2 = tmp;
			}

			// Do 2-point crossover
			pop.Crossover( index1, 
				           index2,
						   point1, 
						   point2,
						   bestIndex );
		}
	}
}


// Mutate selected Tours
void GeneticAlgorithm::Mutate()
{
	for ( int i = 0; i < populationSize; i++ )
	{
		if ( i == bestIndex ) continue;

		int r = rand() % 100;

		if ( r < mutationRate  )
		{
			pop.Mutation( i );
		}
	}
}


// Select population Tours according to fitness
// Using a pairwise tournament selection mechanism
void GeneticAlgorithm::Select()
{
	// For each pair of Tours selected
	int i = 0;

	while ( i < tournamentSize )
	{	
		// Get the Tour pair for tournament selection
		int index1 = rand() % populationSize;
		int index2 = rand() % populationSize;

		while ( index1 == index2 )
		{
			index2 = rand() % populationSize;
		}

		double fitness1 = fabs( pop.GetTourFitness( index1 ) );
		double fitness2 = fabs( pop.GetTourFitness( index2 ) );				
		
		// We seek to find [x,y] that minimizes this function
		// The shorter the tour, the higher the fitness
		if ( fitness1 > fitness2 )
		{
			// Tour 1 wins: Copy Tour 1 elements into Tour 2
			pop.CopyTour( index1, index2 );			
		}
		else
		{
			// Tour 2 wins: Copy Tour 2 elements into Tour 1
			pop.CopyTour( index2, index1 );			
		}

		i++;
	}
}

// Set mutation rate, population size etc
void GeneticAlgorithm::SetParameters( const int& crate,
									  const int& mrate,
									  const int& psize,
									  const int& iter,
									  const int& tsize )
{
	mutationRate = mrate;
	crossoverRate = crate;
	populationSize = psize;
	numberIterations = iter;	
	tournamentSize = tsize;	
}


// Create initial random population of Tours
void GeneticAlgorithm::CreatePopulation()
{
	pop.CreateRandomPopulation( populationSize );
}

// Log the value to a text file
void GeneticAlgorithm::LogResult( const double& result,
	                              const int& iter,
	                              const int& count )
{
	if ( iter % count == 0 )
	{
		std::stringstream ss;
		ss << result;
		log.Write( (char*) ss.str().c_str() );
	}
}