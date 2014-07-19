#pragma once
#include "cantsystem.h"
class CBestWorstAntSystem :
	public CAntSystem
{
	
	int worstIndex;
	int Restart;
	double  trail_0;
	double trail_max;
	double	trail_min;
	int restart_iteration;
	DWORD restart_time;
	DWORD starttime;

public:
	CBestWorstAntSystem(Parameters &Par, MatrixArrayTypeInt *distanceMatrix);
	virtual ~CBestWorstAntSystem(void);

	
public:
	virtual void updatePheromones();
	void initPheromoneTrails(double initialValue) const;
	void bwas_worst_ant_update( const CAnt &bestAnt, const CAnt &worstAnt);
	long distance_between_ants( const CAnt &bestAnt, const CAnt &worstAnt2);
	void bwas_pheromone_mutation(  );
	virtual void initPheromones();

//	void chooseClosestNext(std::vector<bool> &antsvisted , std::vector<size_t> &nntour);
//	void calculateNearestNeigbhor(unsigned int NUMOFANTS);


};

