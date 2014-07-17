#include "StdAfx.h"
#include "Tour.h"
#include <algorithm>
#include <functional>


Tour::Tour(void)
{
}


Tour::~Tour(void)
{
}


Tour::Tour( CoordMatrix* mat )
{


}


Tour::Tour( CoordMatrix* mat, std::vector< int > tour )
{


}

// Get total distance of tour
double Tour::TourDistance( CoordMatrix* mat ) const
{
	double dist = 0.0;

	int size = (int) cities.size();
	int c1, c2;

	for ( int i = 0; i < size - 1; i++ )
	{
		c1 = cities.at( i );
		c2 = cities.at( i + 1 );
		dist += mat->Distance( c1, c2 );
	} 

	// And back to the beginning city
	c1 = cities.at( size - 1 );
	c2 = cities.at( 0 );
	dist += mat->Distance( c1, c2 );

	return dist;
}


// Generate arbitrary tour
void Tour::CreateRandomTour( const int& size )
{
	Reset();

	for ( int i = 0; i < size; i++ )
	{
		cities.push_back( i );
	}

	random_shuffle( cities.begin() + 1, cities.end() );
}


// Get distance bewteen selected cities
double Tour::Distance( const int& c1, 
		               const int& c2,
					   CoordMatrix* mat ) const
{
	return mat->Distance( c1, c2 );
}


// Reset existing tour data
void Tour::Reset()
{
	std::vector< int >().swap( cities );
}

// Return the number of cities in the tour
int Tour::TourSize() const
{
	return (int) cities.size();
}

// Get the tour city, given the index passed to it
int Tour::GetCity( const int& index )
{
	int node = cities.at( index );
	return node;
}


// Get tour from the set of nearest neighbours
void Tour::CreateNearestNeighbourTour( const int& size,
	                                   CoordMatrix* mat )
{
	Reset();

	std::set<int> city_set;
	std::set<int>::iterator it;

	for ( int i = 0; i < size; i++ )
	{
		city_set.insert( i );
	}

	int city = 0;	
	
	for ( int i = 1; i <= size; i++ )
	{
		// Add city to tour
		cities.push_back( city );

		// Remove city from unique set
		it = city_set.find( city );
		city_set.erase( it );

		city = GetNearestNeighbour( city, city_set, mat );		
	}
}

// Get the nearest node to this one
int Tour::GetNearestNeighbour( const int& c, 
	                           std::set<int>& cset,
							   CoordMatrix* mat )
{
	int city = 0;

	// Get minimum distance node
	double min_dist = 99999999;
	std::set<int>::iterator cit;

	 for ( cit = cset.begin(); cit != cset.end(); cit++ )
	 {
		 int n = *cit;

		 if ( n == c ) continue;

		 double dist = Distance( c, n, mat );
		
		 if ( dist < min_dist )
		 {
			city = n;
			min_dist = dist;
		 }
	 }
    
	return city;
}

// Swap selected cities
void Tour::Swap( const int& c1,const int& c2 )
{
	int tmp = cities.at( c1 );
	cities[ c1 ] = cities.at( c2 );
	cities[ c2 ] = tmp;
}


// Do standard 2-opt operation between selected cities
void Tour::TwoOpt( const int& c1, 
				   const int& c2,
				   const int& c3,
				   const int& c4 )
{
	

	// Feasible exchanges only
	if ( c3 == c1 || c3 == c2 || c4 == c1 || c4 == c2 ) return;
		
	// Leave values at positions c1, c4
	// Swap c2, c3 values
	// c3 -> c2
	// c2 -> c3
	int tmp = cities.at( c2 );
	cities[ c2 ] = cities.at( c3 );
	cities[ c3 ] = tmp;	
}


// Randomly scramble the container elements between the two
// given end points
void Tour::Scramble( const int& p1, const int &p2 )
{
	int c1 = p1 < p2 ? p1 : p2;
	int c2 = p1 < p2 ? p2 : p1;

	std::vector< int >::iterator it1 = cities.begin() + c1;
	std::vector< int >::iterator it2 = cities.begin() + c2;

	random_shuffle( it1, it2 );
}


// Randomly rotate the container elements 
void Tour::Rotate( const int& p1, 
	               const int& p2,
				   const int& p3 )
{
	std::vector< int >::iterator it1 = cities.begin() + p1;
	std::vector< int >::iterator it2 = cities.begin() + p2;
	std::vector< int >::iterator it3 = cities.begin() + p3;

	rotate( it1, it2, it3 );
}


void Tour::ReverseCities( const int& c1, const int& c2 )  
{  
    int c1_n = c1 < c2 ? c1 : c2;  
    int c2_n = c1 < c2 ? c2 : c1;  
  
    if ( c1_n == 0 ) c1_n = 1;  
  
    std::vector<int>::iterator it1, it2;  
  
    it1 = cities.begin() + c1_n;  
    it2 = cities.begin() + c2_n;  
  
    reverse( it1, it2 );  
} 


// Copy / clone the tour cities to another vector
void Tour::StoreTour( std::vector<int>& v )
{
	int size = TourSize();
	v.reserve( size );

	v = cities;
}

// Set the cities
void Tour::SetCities( const std::vector<int>& v )
{
	cities = v;
}

// Set fitness of chromosome
void Tour::SetFitness( const double& value )
{
	fitness = value;
}

// Return overall tour fitness
double Tour::CalcTourFitness( CoordMatrix* mat ) const
{
	double distance = TourDistance( mat );
	return 1 / distance;
}

// Get the stored tour fitness
double Tour::GetFitness() const
{
	return fitness;
}

// Set city value
void Tour::SetCity( const int& index, const int& value )
{
	cities[ index ] = value;
}