#pragma once
#include "CAntSystem.h"


class CElitistAntSystem :
	public CAntSystem
{
	int mWeight; 
public:
	virtual void updatePheromones();
	CElitistAntSystem(Parameters& Par , MatrixArrayTypeInt *distanceMatrix);
	virtual ~CElitistAntSystem(void);
};

