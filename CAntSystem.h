#pragma once

#include <vector>
#include <random>
#include <time.h>
#include <math.h>

#include <algorithm>    // std::random_shuffle


#include "CAnt.h"
#include "CLocalSearch.h"
#include "PheroMatrix.h"
#include "Utils.h"

typedef std::vector<std::vector<double> > MatrixArrayTypeDouble;
typedef std::vector<std::vector<int> > MatrixArrayTypeInt;

class CAntSystem
{
	bool m_bLocalSearch , m_LocalSearchOpt2 , m_LocalSearchOpt3 , m_LocalSearchGreed;


protected:
	CAnt m_RestartBestAnt;
	PheroMatrix *m_newPheromoneMatrix;

	CAnt m_WorstAntToDate;
	CAnt m_BestAntToDate;
	//CAnt m_RestartBestAnt;
	CAnt *m_pWorstAntItt, *m_pBestAntItt;


//	MatrixArrayTypeDouble *m_pheromoneMatrix;
	MatrixArrayTypeDouble *m_heuristicMatrix;
	MatrixArrayTypeInt  *m_distanceMatrix;
	long irreationSinceLastBest;
	CLocalSearch *m_pLocalSearch;

//	std::mt19937   m_randomGenerator;
	
	std::vector<CAnt> m_Ants;
	//std::vector<size_t> m_bestSoFarPath;
	//std::vector<size_t> m_worstSoFarPath;
	std::vector<size_t> m_randomPath;
	std::vector<double> t_prob;
	std::vector<double> m_strength;


	//std::vector<size_t> m_bestIrrPath; //refactor
	//std::vector<size_t> m_worstIrrPath; //refactor

	std::vector<std::vector<size_t> > m_nnList;

	//CAnt m_bestAntToDate;

			
    size_t m_noNodes;
    double m_alpha;
    double m_beta;
    double m_rho;
    int m_iterations;
	int m_noAnts;
	//double m_bestSoFarPathlength;
	//double m_worstSoFarPathlength;

	//size_t BestAntIndex;
	//size_t worseindex;

	double fRand(double a, double b)
	{
		static bool seeded = false;
		if(!seeded) {
			srand((unsigned )time(NULL));
			seeded = true;
		}
		long _rnd = rand();
		if (_rnd == 0)
			_rnd++;
		return b * (double)_rnd / RAND_MAX;	
	}


public:

	virtual void initPheromones();
	void initAnts();
	void initMatrix(MatrixArrayTypeInt &distanceMatrix);
	CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix);
	void Create(Parameters& Par);

	

	//std::vector<size_t> &getBestTourIrreration();
	//CAnt & getBestAntIrreration()
	//{
	//	return  m_Ants[BestAntIndex];
	//}
//	std::vector<size_t> &getWorstTourIrreration();

	double calculatePathLength(const std::vector<size_t> &currentPath) const;
	double calculateAntPathLength(CAnt &ant) const
	{
		size_t  start, end;
		double d = 0;
		for ( size_t i=1; i<ant.getNoNodes(); i++)
		{
			start = ant.getCity(i-1);//[i-1];
			end = ant.getCity(i);
			d += (*m_distanceMatrix)[start][end];	
		}
	//	d += (*m_distanceMatrix)[currentPath[m_noNodes-1]][currentPath[0]];
		return d;
	}

	virtual void evaporateAllPhero();
	void adjustPheremone(size_t antPos);
	void decisionRule(size_t k, size_t step);

	//virtual
	virtual void constructSolutions();
	virtual void updateBestSoFarPath();
	virtual void updatePheromones();
	virtual void calculateHeuristicMatrix();

	
	
	virtual ~CAntSystem(void);

	int getIteration() const
	{
		return m_iterations;
	}
	CAnt & getBestAntToDate()
	{
		return this->m_BestAntToDate;
	}

/*	int computeAntsItterationBestIndex() 
	{
		int best= (std::numeric_limits<int>::max)();
		for( int i=0; i<  m_noAnts ; i++)
		{
			int length = m_Ants[i].getAntTourLength();
			if( length < best)
			{
				best =length;
				BestAntIndex = i;
			}
		}
		return best;
	}
	int computeAntsItterationWorstIndexAndLength() 
	{
		int worse=0;
		for(size_t i=0; i< m_noAnts ; i++)
		{
			int length = m_Ants[i].getAntTourLength();
			if( length > worse)
			{
				worse =length;
				worseindex =i;
			}
		}
		return worse;
	}
*/
	const CAnt& getWorstAntTodate() const {
		return m_WorstAntToDate;
	}


//	double getBestSoFarPathLength() const
	//{
//		return m_BestAntToDate.getAntTourLength();
		//return calculatePathLength(m_bestSoFarPath);
//	}
	void incIteration() 
	{
		m_iterations++;
	}
	const std::vector<size_t> &getBestSoFarPath()
	{
		return m_BestAntToDate.getAntsCityTour();
		//return m_bestSoFarPath;
	}
	double getBestPathLengthSofar()
	{
		return m_BestAntToDate.getAntTourLength();
		//return m_bestSoFarPathlength;
	}
//	void setPheromonesMatrix(std::vector<std::vector<double> > &MatrixArrayType)
//	{
		//this->m_pheromoneMatrix = &MatrixArrayType;
//	}
	void setDistance(std::vector<std::vector<int> > &MatrixArrayType)
	{
		this->m_distanceMatrix = &MatrixArrayType;
	}
	const CAnt& getAnt(size_t pos)
	{
      return m_Ants[pos];
    }
//	const CAnt &GetBestAnt()
//	{
//		return getAnt(BestAntIndex);
//	}
//	double getPheromone(int i, int j) const
//	{
 // //      return 0;//(*m_pheromoneMatrix)[i][j];
//    }
//	double getHeuristicAt(int i, int j) const
//	{
 //       return (*m_heuristicMatrix)[i][j];
 //   }
 //   int getDistance(int i, int j) const
//	{
 //       return (*m_distanceMatrix)[i][j];
  //  }

	//************************************
	// Method:    localSearch
	// FullName:  CAntSystem::localSearch
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void localSearch()
	{

		//double length1 = this->calculatePathLength(m_Ants[i].getAntsCityTour());

		if(m_LocalSearchOpt2)
			for(int i = 0; i < m_Ants.size(); i++)
				m_pLocalSearch->opt2(m_Ants[i].getAntsCityTour());
		if(m_LocalSearchOpt3)
			for(int i = 0; i < m_Ants.size(); i++)
				m_pLocalSearch->opt3(m_Ants[i].getAntsCityTour());
		if(m_LocalSearchGreed)
			for(int i = 0; i < m_Ants.size(); i++)
				m_pLocalSearch->two_opt_first(m_Ants[i].getAntsCityTour() , m_nnList);

		//double length2 = calculatePathLength(m_Ants[i].getAntsCityTour());


		updateBestSoFarPath();
	}


	
};

