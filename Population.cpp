#include "stdafx.h"
#include "Population.h"
#include <algorithm>
#include <iterator>


#include <bitset>
#include <iostream>
#include <math.h>

Population::Population(void)
{
}


Population::~Population(void)
{
	int size = (int) pop.size();

	// Delete the Tour pointers
	for ( int i = 0; i < size; i++ )
	{
		Tour* tour = pop.at( i );

		if ( tour )
		{
			delete tour;
		}
	}

	pop.clear();
}


// Create initial arbitrary population of Tours
void Population::CreateRandomPopulation( const int& size )
{
	// Seed the population with a nearest neighbour tour
	//Tour* nntour = CreateNearestNeighbourTour( matrix->size() );
	//pop.push_back( nntour );

	for ( int i = 0; i < size; i++ )
	{	
		Tour* chr = CreateArbitraryTour( matrix->size() );
		pop.push_back( chr );
	}
}


// Apply two-point crossover to selected Tour pair
void Population::Crossover( const int& index1,
						    const int& index2,
						    const int& point )
{
	if ( point < 0 || point >= chrSize ) return;

	
}


// Apply two-point crossover to selected Tour pair
void Population::Crossover( const int& index1,
						    const int& index2,
						    const int& point1,
						    const int& point2,
							const int& bestIndex )
{
	std::vector<int> offspr;

	if ( point1 < 0 || point1 >= chrSize ) return;
	if ( point2 < 0 || point2 >= chrSize ) return;
	 
	int p1 = point1;
	int p2 = point2;
	
	if ( p1 > p2 )
	{
		int tmp = p1;
		p1 = p2;
		p2 = tmp;
	}	

	// Create new offspring to replace parent
	Tour* tour1 = GetTour( index1 );
	Tour* tour2 = GetTour( index2 );

	std::vector<int> s1;
	std::vector<int> s2;

	for ( int i = p1; i < p2; i++ )
	{
		int city1 = tour1->GetCity( i );
		int city2 = tour2->GetCity( i );
		s1.push_back( city1 );
		s2.push_back( city2 );
	}

	std::sort( s1.begin(), s1.end() );
	std::sort( s2.begin(), s2.end() );

	// Find all numbers unique to both s1 and s2
	std::set<int> intersection; 
    std::set_intersection( s1.begin(), 
		                   s1.end(), 
						   s2.begin(),
						   s2.end(), 
						   std::inserter( intersection, intersection.end() ) );

	std::vector<int> s1_new;
	std::vector<int> s2_new;

	int size = p2 - p1;

	for ( int i = 0; i < size; i++ )
	{
		int val1 = s1[ i ];
		int val2 = s2[ i ];

		bool is_in1 = intersection.find(val1) != intersection.end();
		bool is_in2 = intersection.find(val2) != intersection.end();

		if ( !is_in1 )		
		{
			s1_new.push_back( val1 );			
		}

		if ( !is_in2 )		
		{
			s2_new.push_back( val2 );			
		}
	}
	
	int index = 0;

	for ( int i = 0; i < chrSize; i++ )
	{
		if ( i >= p1 && i < p2 )
		{
			int val = tour2->GetCity( i );
			offspr.push_back( val );
		}
		else
		{			
			int val = tour1->GetCity( i );

			// Is val in s2_new set?
			std::vector<int>::iterator it;
			it = find( s2_new.begin(), s2_new.end(), val );

			bool is_in = it != s2_new.end();

			if ( is_in )
			{
				int val_new = s1_new[ index++ ];
				offspr.push_back( val_new );			
			}
			else
			{
				offspr.push_back( val );
			}
		}
	}

	// Copy the values to create one new offspring
	int member = rand() % pop.size();

	while ( member == bestIndex )
	{
		member = rand() % pop.size();
	}

	Tour* tour = pop.at( member );

	tour->SetCities( offspr );
}


// Apply mutation to selected Tour
void Population::Mutation( const int& index )
{	
	Tour* tour = pop.at( index );
	int tsize = tour->TourSize();

	int p1 = 1 + rand() % ( tsize - 1 );
	int p2 = 1 + rand() % ( tsize - 1 );

	while ( p1 == p2 )
	{
		p2 = 1 + rand() % ( tsize - 1 );
	}

	int r = rand() % 100;

	if ( r < 33 )
	{
		int n = 1 + rand() % 3;
		
		for ( int i = 0; i < n; i++ )
		{
			p1 = 1 + rand() % ( tsize - 1 );
			p2 = 1 + rand() % ( tsize - 1 );

			while ( p1 == p2 )
			{
				p2 = 1 + rand() % ( tsize - 1 );
			}

			tour->Swap( p1, p1 );
		}
	}
	else if ( r < 67 )
	{
		int c[ 3 ];

		c[ 0 ] = 1 + rand() % ( tsize - 1 );
		c[ 1 ] = 1 + rand() % ( tsize - 1 );
		c[ 2 ] = 1 + rand() % ( tsize - 1 );

		while ( c[ 0 ] == c[ 1 ] || c[ 1 ] == c[ 2 ] || c[ 1 ] == c[ 2 ] ) 
		{
			c[ 0 ] = 1 + rand() % ( tsize - 1 );
			c[ 1 ] = 1 + rand() % ( tsize - 1 );
			c[ 2 ] = 1 + rand() % ( tsize - 1 );
		}

		std::sort( c, c + 3 );
		tour->Rotate( c[ 0 ], c[ 1 ], c[ 2 ] );
	}
	else
	{
		tour->ReverseCities( p1, p2 );
	}
}


// Get tour pointer
Tour* Population::GetTour( const int& index )
{
	return pop.at( index );
}

// Evaluate the population fitnesses
int Population::EvaluatePopulation()
{	
	double bestFitness = infinity * -1;
	int bestFitnessIndex = 0;

	for ( int i = 0; i < (int) pop.size(); i++ )
	{	
		Tour* tour = pop.at( i );
		double fitness = tour->CalcTourFitness( matrix );
		tour->SetFitness( fitness );		

		if ( fitness > bestFitness )
		{
			bestFitness = fitness;
			bestFitnessIndex = i;
		}
	}
	
	return bestFitnessIndex;
}



// Create an arbitrary random Tour
Tour* Population::CreateArbitraryTour( const int& size )
{
	Tour* tour = new Tour();	
	tour->CreateRandomTour( size );

	return tour;
}

// Create an arbitrary random Tour
Tour* Population::CreateNearestNeighbourTour( const int& size )
{
	Tour* tour = new Tour();	
	tour->CreateNearestNeighbourTour( size, matrix );

	return tour;
}


// Set pointer to cost / distance matrix object
void Population::SetMatrix( CoordMatrix* mat )
{
	matrix = mat;
}


// Get the fitness of the selected Tour
double Population::CalcTourFitness( CoordMatrix* mat )
{	
	return 0.0;
}


// Set the size of the Tour
void Population::SetTourSize( const int& size )
{
	chrSize = size;
}


// Get fitness of selected Tour
double Population::GetTourFitness( const int& index ) const
{
	Tour* tour = pop.at( index );
	return tour->GetFitness();	
}


// Copy the contents of the source Tour into destination
void Population::CopyTour( const int& source, const int& dest )
{
	// Get the chromosomes
	Tour* tour1 = pop.at( source );
	Tour* tour2 = pop.at( dest );

	// Copy elements and fitness of source chromosome 
	// into the destination chromosome
	for ( int i = 0; i < chrSize; i++ )
	{
		// Get source chromosome element
		unsigned char value = tour1->GetCity( i );

		// Write this element value into the destination element
		tour2->SetCity( i, value );
	}

	// Set the destination chromosome fitness
	double fitness = tour1->GetFitness();
	tour2->SetFitness( fitness );	
}