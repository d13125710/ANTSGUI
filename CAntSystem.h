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

using namespace std;


class CAntSystem
{

	
	void calculateNearestNeigbhor(unsigned int NUMOFANTS=20);

protected:
	bool m_bLocalSearch , m_LocalSearchOpt2 , m_LocalSearchOpt3 , m_LocalSearchGreed;
	std::mt19937 g_rndTravel;
	CAnt m_RestartBestAnt;
	PheroMatrix *m_newPheromoneMatrix;
	CLocalSearch *m_pLocalSearch;

	CAnt m_WorstAntToDate;
	CAnt m_BestAntToDate;
	CAnt *m_pWorstAntItt, *m_pBestAntItt;

	MatrixArrayTypeDouble *m_heuristicMatrix;
	MatrixArrayTypeInt  *m_distanceMatrix;
	long irreationSinceLastBest;
	

	std::vector<CAnt> m_Ants;
	std::vector<size_t> m_randomPath;
	std::vector<double> t_prob;
	std::vector<double> m_strength;


	std::vector<std::vector<size_t> > m_nnList;

	
			
    size_t m_noNodes;
    double m_alpha;
    double m_beta;
    double m_rho;
    int m_iterations;
	int m_noAnts;




protected:


	void initMatrix(MatrixArrayTypeInt &distanceMatrix);
	void Create(Parameters& Par);
	double calculatePathLength(const std::vector<size_t> &currentPath) const;
	double calculatePathLength2(const std::vector<size_t> &currentPath) const;
	double calculateAntPathLength(CAnt &ant) const {
		size_t  start, end;
		double d = 0;
		for ( size_t i=1; i<ant.getNoNodes(); i++)
		{
			start = ant.getCity(i-1);//[i-1];
			end = ant.getCity(i);
			d += (*m_distanceMatrix)[start][end];	
		}
		return d;
	}
	
	
	
	virtual void evaporateAllPhero();
	void adjustPheremone(size_t antPos);
	void decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh);

	//virtual
public:
	CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix);
	void initAnts();
	virtual void constructSolutions();
	virtual bool updateBestSoFarPath();
	virtual void updatePheromones();
	virtual void calculateHeuristicMatrix();	
	virtual ~CAntSystem(void);
	virtual void initPheromones();

	void incIteration() {
		m_iterations++;
	}

	int getIteration() const{
		return m_iterations;
	}
	const std::vector<size_t> &getBestSoFarPath(){
		return m_BestAntToDate.getAntsCityTour();
	}
	double getBestPathLengthSofar()	{
		return m_BestAntToDate.getAntTourLength();
	}
protected:
	CAnt & getBestAntToDate(){
		return this->m_BestAntToDate;
	}
	const CAnt& getWorstAntTodate() const {
		return m_WorstAntToDate;
	}



	const CAnt& getAnt(size_t pos) const{
      return m_Ants[pos];
    }
	//************************************
	// Method:    localSearch
	// FullName:  CAntSystem::localSearch
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
public:

	void localSearch()
	{

		if(m_LocalSearchOpt2)
			for(int i = 0; i < m_Ants.size(); i++)
			{
				m_pLocalSearch->opt2(m_Ants[i].getAntsCityTour());
				m_Ants[i].setAntTourLength((int)this->calculateAntPathLength(m_Ants[i]));
			}
		if(m_LocalSearchOpt3)
			for(int i = 0; i < m_Ants.size(); i++)
			{
				m_pLocalSearch->opt3(m_Ants[i].getAntsCityTour());
				m_Ants[i].setAntTourLength((int)this->calculateAntPathLength(m_Ants[i]));
			}
		if(m_LocalSearchGreed)
			for(int i = 0; i < m_Ants.size(); i++)
			{
				m_pLocalSearch->three_opt_first(m_Ants[i].getAntsCityTour() , m_nnList);
				m_Ants[i].setAntTourLength((int)this->calculateAntPathLength(m_Ants[i]));
			}

		updateBestSoFarPath();
	}


	
};

