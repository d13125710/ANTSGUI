#pragma once

#include <vector>
#include "Tour.h"
#include "CoordMatrix.h"

const double infinity = 9999999999999;

class Population
{
public:
	Population(void);
	~Population(void);

	void SetTourSize( const int& size );
	void CreateRandomPopulation( const int& size );	

	void Crossover( const int& index1,
		            const int& index2,
					const int& point );

	void Crossover( const int& index1,
		            const int& index2,
					const int& point1,
					const int& point2,
					const int& bestIndex );

	void Mutation( const int& index );
	int EvaluatePopulation();

	double CalcTourFitness( CoordMatrix* mat );

	double GetTourFitness( const int& index ) const;
	void CopyTour( const int& source, const int& dest );
	void SetMatrix( CoordMatrix* mat );
	Tour* GetTour( const int& index );


private:
	Tour* CreateArbitraryTour( const int& size );	
	Tour* CreateNearestNeighbourTour( const int& size );
	
private:
	std::vector< Tour* > pop;
	CoordMatrix* matrix;
	int chrSize;
};