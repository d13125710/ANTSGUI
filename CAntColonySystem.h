#pragma once
#include "CAntSystem.h"
#include "CLocalSearch.h"
#include "Utils.h"

class CAntColonySystem :
	public CAntSystem
{
	std::mt19937 m_MExplore;
	double xi;
	double tau0;
	double q0;
	double m_r0;



private:
	void CalculateHeuristicForNode(size_t i, size_t j);
	virtual void decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh, uniform_real<double> &choiceE);
	virtual void evaporateAllPhero();
	double HEURISTIC(size_t m, size_t n) {
		return (1.0 / ((double) (*m_distanceMatrix)[m][n] + 0.000000001));
	}
	void globalPheromoneDeposit();
	virtual bool updateBestSoFarPath();
	void localPheromoneEndIndexUpdate(size_t idx1, size_t idx2);
	void localPheromoneUpdate(size_t ant, size_t step);
	//void calculateNearestNeigbhor(unsigned int numberOfAnts);
	void chooseClosestNext(std::vector<bool> &antsvisted , std::vector<size_t> &nntour);
	void decisionRule2(size_t k, size_t step);
	void choose_closest_next(CAnt& a, size_t phase);
	void neighbour_choose_best_next(CAnt &a, size_t phase);
	void choose_best_next(CAnt& a, size_t phase);
public:
	virtual void calculateHeuristicMatrix();
	virtual void constructSolutions();
	virtual void updatePheromones();
	virtual void initPheromones();
	CAntColonySystem(Parameters &Par, std::vector<std::vector<int> > *matrix);
	~CAntColonySystem(void);
};

