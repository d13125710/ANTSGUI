#pragma once

#include <vector>
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include <limits>       


#include "CAnt.h"
#include "CAntSystem.h"
#include "CLocalSearch.h"


class CMinMaxAntSystem: public CAntSystem
{
private:

	double m_tau0;
	double m_lambda;
	double  m_trail_0;
	double  m_trail_max;
	double	m_trail_min;
	bool m_resetAnt;
	double m_restartBestAntTourLength;
	std::vector<size_t> m_restartAntBestPath;

private:

	void initPheromoneTrails(double initialValue) const;
	void globalUpdatePheromone(const std::vector<size_t> &AntTour);
 	double nodeBranching(double l) const;
	virtual bool updateBestSoFarPath();

public:
	
	virtual void initPheromones();
	void checkPheromoneLimits();
	void updatePheromones();
	CMinMaxAntSystem(Parameters& Par , std::vector<std::vector<int> >  *Vdistance);
	virtual ~CMinMaxAntSystem(void);
};

