#pragma once

#include "CoordMatrix.h"
#include <vector>
#include <set>


class Tour
{
public:
	Tour(void);
	Tour( CoordMatrix* mat );
	Tour( CoordMatrix* mat, std::vector< int > tour );
	~Tour(void);

	void Swap( const int& c1,const int& c2 );
	
	void TwoOpt( const int& c1, 
				 const int& c2,
				 const int& c3,
				 const int& c4 );
	void Scramble( const int& p1, const int &p2 );

	void Rotate( const int& p1, const int& p2, const int& p3 );
	void ReverseCities( const int& c1, const int& c2 );

	double CalcTourFitness( CoordMatrix* mat ) const;
	
	int TourSize() const;
	int GetCity( const int& index );
	void StoreTour( std::vector<int>& v );

	void SetMatrix( CoordMatrix* mat );
	void CreateRandomTour( const int& size );
	
	void CreateNearestNeighbourTour( const int& size, CoordMatrix* mat );
	void SetCities( const std::vector<int>& v );
	void SetCity( const int& index, const int& value );
	void Reset();
	void SetFitness( const double& value );
	double GetFitness() const;
	double TourDistance( CoordMatrix* mat ) const;

private:	

	double Distance( const int& c1, 
		             const int& c2,
					 CoordMatrix* mat ) const;	

	int GetNearestNeighbour( const int& c, 
		                     std::set<int>& cset,
							 CoordMatrix* mat );	

private:
	std::vector< int > cities;	
	double fitness;
};

