#pragma once

#include "Population.h"
#include "Log.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(void);
	~GeneticAlgorithm(void);

	void Initialize( const int& crate,
					 const int& mrate,
					 const int& psize,
					 const int& iter,
					 const int& csize,					 
					 const std::string& path,
					 CoordMatrix* mat );	
	
	Tour* Run( const int& iter,
		       CoordMatrix* mat );

private:

	void CreatePopulation();
	int  Evaluate();
	void Crossover();
	void Mutate();
	void Select();

	void SetParameters( const int& crate,
		                const int& mrate,
						const int& psize,
						const int& iter,
						const int& tsize );

	void LogResult( const double& result, 
		            const int& iter,
		            const int& count );

private:
	int mutationRate;
	int crossoverRate;
	int populationSize;
	int numberIterations;
	int TourSize;
	int tournamentSize;

	int bestFitnessIndex;
	double bestFitness;
	int bestIndex;

	Population pop;
	Log log;
};

