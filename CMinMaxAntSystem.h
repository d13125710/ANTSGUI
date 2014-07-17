#pragma once

#include <vector>
#include <random>
#include <time.h>
#include <math.h>

#include "CAnt.h"
#include "CAntSystem.h"
#include "CLocalSearch.h"




class CMinMaxAntSystem: public CAntSystem
{
private:
	double m_tau0;
	bool m_doLocalSearch , m_doOpt2 , m_doOpt3;
    int m_maxStagnationIterations , m_stagnationIterations;
	double lambda;
	double  m_branchingFactor;
	double  trail_0;
	double trail_max;
	double	trail_min;
	bool m_resetAnt;
	double m_restartBestAntTourLength;
	double m_bestIterationLength;
	
	std::vector<size_t> m_restartAntBestPath;
	std::vector<size_t> m_bestIterationAntPath;
	
	//added
private:
	//std::vector<std::vector<int> > m_nnList;
	void calculateNearestNeigbhor(unsigned int numberOfAnts);
	void initPheromoneTrails(double initialValue) const;
	void globalUpdatePheromone(const std::vector<size_t> &AntTour);
 	void chooseClosestNext(std::vector<bool> &antsvisted , std::vector<size_t> &nntour);
	double nodeBranching(double l);


public:
	virtual void updateBestSoFarPath();
	void initPheromones();
	void checkPheromoneLimits();
	void updatePheromones();
	CMinMaxAntSystem(Parameters& Par , std::vector<std::vector<int> >  *Vdistance);
	virtual ~CMinMaxAntSystem(void);
};

