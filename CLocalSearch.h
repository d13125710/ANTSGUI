#pragma once

#include <vector>
#include <limits>       // std::numeric_limits
#include <algorithm>    // std::random_shuffle


typedef std::vector<std::vector<int> > MatrixArrayTypeInt;

using namespace std;

class CLocalSearch
{
	MatrixArrayTypeInt  *m_Vdistance;

public:

	//************************************
	// Method:    CLocalSearch
	// FullName:  CLocalSearch::CLocalSearch
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	CLocalSearch(){
		m_Vdistance = 0;
	}

	//************************************
	// Method:    CLocalSearch
	// FullName:  CLocalSearch::CLocalSearch
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: MatrixArrayTypeInt * Vdistance
	//************************************
	CLocalSearch(MatrixArrayTypeInt  *Vdistance){
		m_Vdistance = Vdistance;

	}
	void setMatrix(MatrixArrayTypeInt  *Vdistance){
		if(m_Vdistance == 0){
			m_Vdistance = Vdistance;
		}
	}


	void opt2(std::vector<size_t> &tour)
	 {
        size_t i;
        int a1,a2,a3,b1,b2,b3; size_t swap;
        for(i = 1; i < tour.size()-2; i++)
		{
            a1 = (*m_Vdistance)[tour[i-1]][tour[i]];
            a2 = (*m_Vdistance)[tour[i]][tour[i+1]];
            a3 = (*m_Vdistance)[tour[i+1]][tour[i+2]];
            b1 = (*m_Vdistance)[tour[i-1]][tour[i+1]];
            b2 = (*m_Vdistance)[tour[i+1]][tour[i]];
            b3 = (*m_Vdistance)[tour[i]][tour[i+2]];
            if(a1 + a2 + a3 > b1 + b2 + b3)
			{
                swap = tour[i];
                tour[i] = tour[i+1];
                tour[i+1] = swap;
            }
		}
    }

	   void opt3(std::vector<size_t> &tour)
	   {

        size_t i,j;
        int distances[]={0,0,0,0,0,0};
        for(i = 1; i < tour.size() - 3; i++){
            // ABC
            distances[0] = (*m_Vdistance)[tour[i-1]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i+3]];
            // ACB
            distances[1] = (*m_Vdistance)[tour[i-1]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i+3]];
            // BAC
            distances[2] = (*m_Vdistance)[tour[i-1]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i+3]];
            // BCA
            distances[3] = (*m_Vdistance)[tour[i-1]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+3]];
            // CAB
            distances[4] = (*m_Vdistance)[tour[i-1]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i+3]];
            // CBA
            distances[5] = (*m_Vdistance)[tour[i-1]][tour[i+2]]+(*m_Vdistance)[tour[i+2]][tour[i+1]]+(*m_Vdistance)[tour[i+1]][tour[i]]+(*m_Vdistance)[tour[i]][tour[i+3]];
            // caut min
            int min =  (std::numeric_limits<int>::max)();
            size_t minIdx = -1;
            for(j = 0; j < 6; j++){
                if (min > distances[j]){
                    min = distances[j];
                    minIdx = j;
                }
            }
            switch(minIdx){
                case 0:     // ABC
                    break;
                case 1:     // ACB
                    swapV(tour, i+1, i+2);
                    break;
                case 2:     // BAC
                    swapV(tour, i, i+1);
                    break;
                case 3:     // BCA
                    // ABC -> BAC
                    swapV(tour, i, i+1);
                    // BAC -> BCA
                    swapV(tour, i+1, i+2);
                    break;
                case 4:     // CAB
                    // ABC -> CBA
                    swapV(tour, i, i+2);
                    // CBA -> CAB
                    swapV(tour, i+1, i+2);
                    break;
                case 5:     // CBA
                    swapV(tour, i, i+2);
                    break;

            }
            
        }
	}
    void swapV(std::vector<size_t> &tour , size_t i, size_t j)
	{
        size_t swap = tour[i];
        tour[i] = tour[j];
        tour[j] = swap;
    }

	std::vector<size_t> greedyPath(int noNodes)
	{
        std::vector<bool> visted(noNodes);
		std::vector<size_t> tour(noNodes+1);
		
	    int min, node;
        int i,j;

        for(i = 0; i < noNodes; i++)
            visted[i] = false;

        tour[0] = 0;
     
        visted[0] = true;

        for(i = 1; i < noNodes; i++){
            min = (std::numeric_limits<int>::max)();
            node = -1;
            for(j = 0; j < noNodes; j++){
                if((!visted[j])&&(j!=tour[i-1])){
                    if(min > (*m_Vdistance)[tour[i-1]][j]){
                       min = (*m_Vdistance)[tour[i-1]][j];
                       node = j;
                    }
                }
            }
            tour[i] = node;
            visted[node] = true;
        }
	   tour[noNodes] = tour[0];
       return tour;
   }
	//from thomas strudlez
	 void swap2(double *v, int *v2, int i, int j)
    /*
     * FUNCTION: auxiliary routine for sorting an integer array
     * INPUT: two arraya, two indices
     * OUTPUT: none
     * (SIDE)EFFECTS: elements at position i and j of the two arrays are swapped
     */
    {
		double tmp;

		tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
		tmp = v2[i];
		v2[i] = v2[j];
		v2[j] = static_cast<int>(tmp);
    }

    void sort2(double *v, int *v2, int left, int right)
    /*
     * FUNCTION: recursive routine (quicksort) for sorting one array; second
     * arrays does the same sequence of swaps
     * INPUT: two arrays, two indices
     * OUTPUT: none
     * (SIDE)EFFECTS: elements at position i and j of the two arrays are swapped
     */
    {
		int k, last;

		if (left >= right)
			return;
		swap2(v, v2, left, (left + right) / 2);
		last = left;
		for (k = left + 1; k <= right; k++)
			if (v[k] < v[left])
			swap2(v, v2, ++last, k);
		swap2(v, v2, left, last);
		sort2(v, v2, left, last);
		sort2(v, v2, last + 1, right);
    }

	/* Version: 1.0
		* File: ls.c
		* Author: Thomas Stuetzle
		* Purpose: implementation of local search routines
		* Check: README and gpl.txt
		* Copyright (C) 1999 Thomas Stuetzle
		*/
	 std::vector<size_t> generate_random_permutation(size_t n)
    /*
     * FUNCTION: generate a random permutation of the integers 0 .. n-1
     * INPUT: length of the array
     * OUTPUT: pointer to the random permutation
     * (SIDE)EFFECTS: the array holding the random permutation is allocated in this
     * function. Don't forget to free again the memory!
     * COMMENTS: only needed by the local search procedures
     */
    {
		std::vector<size_t> randomPath(n-1);
		for (size_t i=0; i<n-1; i++)
			randomPath[i]=i;

		//randomPath[m_noNodes]=0;

		std::random_shuffle( randomPath.begin(), randomPath.end() );
		return randomPath;
    }

	void two_opt_first(std::vector<size_t>& tour , std::vector<std::vector<size_t> > &nn_list)
    /*
     * FUNCTION: 2-opt a tour
     * INPUT: pointer to the tour that undergoes local optimization
     * OUTPUT: none
     * (SIDE)EFFECTS: tour is 2-opt
     * COMMENTS: the neighbouAnts.rhood is scanned in random order (this need
     * not be the best possible choice). Concerning the speed-ups used
     * here consult, for example, Chapter 8 of
     * Holger H. Hoos and Thomas Stuetzle,
     * Stochastic Local Search---Foundations and Applications,
     * Morgan Kaufmann Publishers, 2004.
     * or some of the papers online available from David S. Johnson.
     */
    {
		size_t N = tour.size();
		bool gotoExchange = false;

		size_t nn_ls=0;

		size_t c1, c2; /* cities considered for an exchange */
		size_t s_c1, s_c2; /* successor cities of c1 and c2 */
		size_t p_c1, p_c2; /* predecessor cities of c1 and c2 */
		size_t pos_c1, pos_c2; /* positions of cities c1, c2 */
		size_t i, j, h, l;
		size_t help;
		bool improvement_flag;
		size_t h1 = 0, h2 = 0, h3 = 0, h4 = 0;
		int radius; /* radius of nn-search */
		int gain = 0;
		std::vector<size_t> random_vector;
	


		
		 bool dlb_flag = true; 
		
		

		improvement_flag = true;
		random_vector = generate_random_permutation(tour.size());


	//	tour.resize(tour.size()-1);
		std::vector<size_t> pos(N+1); /* positions of cities in tour */
		std::vector<bool> dlb(N+1); /* vector containing don't look bits */

		for (i = 0; i < N; i++) {
			pos[tour[i]] = i;
			dlb[i] = false;
		}


		while (improvement_flag) {

			improvement_flag = false;

			for (l = 0; l < N; l++) {

				c1 = random_vector[l];
				// DEBUG ( assert ( c1 < tour.size() && c1 >= 0); )
				if (dlb_flag && dlb[c1])
					continue;
				pos_c1 = pos[c1];
				s_c1 = tour[pos_c1 + 1];
				radius = (*m_Vdistance)[c1][s_c1];

				/* First search for c1's nearest neighbours, use successor of c1 */
				for (h = 0; h < nn_ls; h++) {
					c2 = nn_list[c1][h]; /* exchange partner, determine its position */
					if (radius > (*m_Vdistance)[c1][c2]) {
						s_c2 = tour[pos[c2] + 1];
						gain =(int)(-radius + (*m_Vdistance)[c1][c2] + (*m_Vdistance)[s_c1][s_c2]- (*m_Vdistance)[c2][s_c2]);
						if (gain < 0) {
							h1 = c1;
							h2 = s_c1;
							h3 = c2;
							h4 = s_c2;
							gotoExchange = true;
							break;
						}
					} else
						break;
				}

				if (gotoExchange) {
					/* Search one for next c1's h-nearest neighbours, use predecessor c1 */
					if (pos_c1 > 0)
						p_c1 = tour[pos_c1 - 1];
					else
						p_c1 = tour[N];
					radius =(*m_Vdistance)[p_c1][c1];
					for (h = 0; h < nn_ls; h++) {
						c2 = nn_list[c1][h]; /* exchange partner, determine its position */
						if (radius > (*m_Vdistance)[c1][c2]) {
							pos_c2 = pos[c2];
							if (pos_c2 > 0)
								p_c2 = tour[pos_c2 - 1];
							else
								p_c2 = tour[N];
							if (p_c2 == c1)
								continue;
							if (p_c1 == c2)
								continue;
							gain = (int)(-radius + (*m_Vdistance)[c1][c2] + (*m_Vdistance)[p_c1][p_c2] - (*m_Vdistance)[p_c2][c2]);
							if (gain < 0) {
								h1 = p_c1;
								h2 = c1;
								h3 = p_c2;
								h4 = c2;
								gotoExchange = true;
								break;
							}
						} else
							break;
					}
				}

				if (!gotoExchange) {
					/* No exchange */
					dlb[c1] = true;
					continue;
				}

				if (gotoExchange) {
					gotoExchange = false;
					improvement_flag = true;
					dlb[h1] = false;
					dlb[h2] = false;
					dlb[h3] = false;
					dlb[h4] = false;
					/* Now perform move */
					if (pos[h3] < pos[h1]) {
						help = h1;
						h1 = h3;
						h3 = help;
						help = h2;
						h2 = h4;
						h4 = help;
					}
					if (pos[h3] - pos[h2] < N / 2 + 1) {
						/* reverse inner part from pos[h2] to pos[h3] */
						i = pos[h2];
						j = pos[h3];
						while (i < j) {
							c1 = tour[i];
							c2 = tour[j];
							tour[i] = c2;
							tour[j] = c1;
							pos[c1] = j;
							pos[c2] = i;
							i++;
							j--;
						}
					} else {
						/* reverse outer part from pos[h4] to pos[h1] */
						i = pos[h1];
						j = pos[h4];
						if (j > i)
							help = N- (j - i) + 1;
						else
							help = (i - j) + 1;
						help = help / 2;
						for (h = 0; h < help; h++) {
							c1 = tour[i];
							c2 = tour[j];
							tour[i] = c2;
							tour[j] = c1;
							pos[c1] = j;
							pos[c2] = i;
							i--;
							j++;
							if (i < 0)
								i = N - 1;
							if (j >= N-1)
								j = 0;
						}
						tour[N] = tour[0];
					}
				} else {
					dlb[c1] = true;
				}

			}
		}

	//	tour.resize(tour.size()+1);
	//	tour[tour.size()-1]=tour[0];
	
}


void three_opt_first( std::vector<size_t>& tour , std::vector<std::vector<size_t> > &nn_list )

/*    
      FUNCTION:       3-opt the tour
      INPUT:          pointer to the tour that is to optimize
      OUTPUT:         none
      (SIDE)EFFECTS:  tour is 3-opt
      COMMENT:        this is certainly not the best possible implementation of a 3-opt 
                      local search algorithm. In addition, it is very lengthy; the main 
		      reason herefore is that awkward way of making an exchange, where 
		      it is tried to copy only the shortest possible part of a tour. 
		      Whoever improves the code regarding speed or solution quality, please 
		      drop me the code at stuetzle no@spam ulb.ac.be
		      The neighbourhood is scanned in random order (this need 
                      not be the best possible choice). Concerning the speed-ups used 
		      here consult, for example, Chapter 8 of
		      Holger H. Hoos and Thomas Stuetzle, 
		      Stochastic Local Search---Foundations and Applications, 
		      Morgan Kaufmann Publishers, 2004.
		      or some of the papers available online from David S. Johnson.
*/
{
    /* In case a 2-opt move should be performed, we only need to store opt2_move = TRUE,
       as h1, .. h4 are used in such a way that they store the indices of the correct move */

    long int   c1, c2, c3;           /* cities considered for an exchange */
    long int   s_c1, s_c2, s_c3;     /* successors of these cities        */
    long int   p_c1, p_c2, p_c3;     /* predecessors of these cities      */   
    long int   pos_c1, pos_c2, pos_c3;     /* positions of cities c1, c2, c3    */
    long int   i, j, h, g, l;
    long int   improvement_flag, help;
    long int   h1=0, h2=0, h3=0, h4=0, h5=0, h6=0; /* memorize cities involved in a move */
    long int   diffs, diffp;
    long int   between = FALSE; 
    long int   opt2_flag;  /* = TRUE: perform 2-opt move, otherwise none or 3-opt move */
    long int   move_flag;  /* 
			      move_flag = 0 --> no 3-opt move 
			      move_flag = 1 --> between_move (c3 between c1 and c2)
			      move_flag = 2 --> not_between with successors of c2 and c3
			      move_flag = 3 --> not_between with predecessors of c2 and c3
			      move_flag = 4 --> cyclic move 
			   */
    long int gain, move_value, radius, add1, add2;
    long int decrease_breaks;    /* Stores decrease by breaking two edges (a,b) (c,d) */
    long int val[3];
    long int n1, n2, n3;
    long int *pos;               /* positions of cities in tour */ 
    long int *dlb;               /* vector containing don't look bits */ 
    long int *h_tour;            /* help vector for performing exchange move */ 
    long int *hh_tour;           /* help vector for performing exchange move */ 
   // long int *random_vector;

	size_t n = tour.size()-1;
	bool dlb_flag=true;
	long int nn_ls=nn_list[0].size()-1;

    pos = (long int *) malloc(n * sizeof(long int));
    dlb = (long int *) malloc(n * sizeof(long int));
    h_tour =(long int *)  malloc(n * sizeof(long int));
    hh_tour = (long int *) malloc(n * sizeof(long int));

    for ( i = 0 ; i < n ; i++ ) {
	pos[tour[i]] = i;
	dlb[i] = FALSE;
    }
    improvement_flag = TRUE;
    std::vector<size_t> random_vector = generate_random_permutation( n+1 );

    while ( improvement_flag ) {
	move_value = 0;
	improvement_flag = FALSE;

	for ( l = 0 ; l < n ; l++ ) {

	    c1 = random_vector[l];
	    if ( dlb_flag && dlb[c1] )
		continue;
	    opt2_flag = FALSE;

	    move_flag = 0;
	    pos_c1 = pos[c1];
	    s_c1 = tour[pos_c1+1];
	    if (pos_c1 > 0)
		p_c1 = tour[pos_c1-1];
	    else 
		p_c1 = tour[n-1];

	    h = 0;    /* Search for one of the h-nearest neighbours */
	    while ( h < nn_ls ) {

		c2   = nn_list[c1][h];  /* second city, determine its position */
		pos_c2 = pos[c2];
		s_c2 = tour[pos_c2+1];
		if (pos_c2 > 0)
		    p_c2 = tour[pos_c2-1];
		else 
		    p_c2 = tour[n-1];
	  
		diffs = 0; diffp = 0;

		radius = (*m_Vdistance)[c1][s_c1];
		add1   = (*m_Vdistance)[c1][c2];

		/* Here a fixed radius neighbour search is performed */
		if ( radius > add1 ) {
		    decrease_breaks = - radius - (*m_Vdistance)[c2][s_c2];
		    diffs =  decrease_breaks + add1 + (*m_Vdistance)[s_c1][s_c2];
		    diffp =  - radius - (*m_Vdistance)[c2][p_c2] + 
			(*m_Vdistance)[c1][p_c2] + (*m_Vdistance)[s_c1][c2];
		}
		else 
		    break;
		if ( p_c2 == c1 )  /* in case p_c2 == c1 no exchange is possible */
		    diffp = 0;
		if ( (diffs < move_value) || (diffp < move_value) ) {
		    improvement_flag = TRUE; 
		    if (diffs <= diffp) { 
			h1 = c1; h2 = s_c1; h3 = c2; h4 = s_c2; 
			move_value = diffs; 
			opt2_flag = TRUE; move_flag = 0;
			/*     	    goto exchange; */
		    } else {
			h1 = c1; h2 = s_c1; h3 = p_c2; h4 = c2; 
			move_value = diffp;  
			opt2_flag = TRUE; move_flag = 0;
			/*     	    goto exchange; */
		    }
		}
		/* Now perform the innermost search */
		g = 0;
		while (g < nn_ls) {
	  
		    c3   = nn_list[s_c1][g];
		    pos_c3 = pos[c3];
		    s_c3 = tour[pos_c3+1];
		    if (pos_c3 > 0)
			p_c3 = tour[pos_c3-1];
		    else 
			p_c3 = tour[n-1];
		  
		    if ( c3 == c1 ) {
			g++;
			continue;
		    }
		    else {
			add2 = (*m_Vdistance)[s_c1][c3];
			/* Perform fixed radius neighbour search for innermost search */
			if ( decrease_breaks + add1 < add2 ) {
			  
			    if ( pos_c2 > pos_c1 ) {
				if ( pos_c3 <= pos_c2 && pos_c3 > pos_c1 )
				    between = TRUE;
				else 
				    between = FALSE;
			    }
			    else if ( pos_c2 < pos_c1 )
				if ( pos_c3 > pos_c1 || pos_c3 < pos_c2 )
				    between = TRUE;
				else 
				    between = FALSE;
			    else {
				printf(" Strange !!, pos_1 %ld == pos_2 %ld, \n",pos_c1,pos_c2);
			    }
			  
			    if ( between ) {
				/* We have to add edges (c1,c2), (c3,s_c1), (p_c3,s_c2) to get 
				   valid tour; it's the only possibility */
			      
				gain = decrease_breaks - (*m_Vdistance)[c3][p_c3] +
				    add1 + add2 +
				    (*m_Vdistance)[p_c3][s_c2];
			      
				/* check for improvement by move */
				if ( gain < move_value ) {
				    improvement_flag = TRUE; /* g = neigh_ls + 1; */
				    move_value = gain;
				    opt2_flag = FALSE;
				    move_flag = 1;
				    /* store nodes involved in move */
				    h1 = c1; h2 = s_c1; h3 = c2; h4 = s_c2; h5 = p_c3; h6 = c3;
				    goto exchange;
				} 
			    }
			    else {   /* not between(pos_c1,pos_c2,pos_c3) */
			      
				/* We have to add edges (c1,c2), (s_c1,c3), (s_c2,s_c3) */
			      
				gain = decrease_breaks - (*m_Vdistance)[c3][s_c3] +
				    add1 + add2 + 
				    (*m_Vdistance)[s_c2][s_c3];
			      
				if ( pos_c2 == pos_c3 ) {
				    gain = 20000;
				}
			      
				/* check for improvement by move */
				if ( gain < move_value ) {
				    improvement_flag = TRUE; /* g = neigh_ls + 1; */
				    move_value = gain;
				    opt2_flag = FALSE;
				    move_flag = 2;
				    /* store nodes involved in move */
				    h1 = c1; h2 = s_c1; h3 = c2; h4 = s_c2; h5 = c3; h6 = s_c3;
				    goto exchange;
				}
			      
				/* or add edges (c1,c2), (s_c1,c3), (p_c2,p_c3) */
				gain = - radius - (*m_Vdistance)[p_c2][c2] 
				    - (*m_Vdistance)[p_c3][c3] +
				    add1 + add2 + 
				    (*m_Vdistance)[p_c2][p_c3];
			      
				if ( c3 == c2 || c2 == c1 || c1 == c3 || p_c2 == c1 ) {
				    gain = 2000000;
				}
			      
				if ( gain < move_value ) {
				    improvement_flag = TRUE;
				    move_value = gain;
				    opt2_flag = FALSE;
				    move_flag = 3;
				    h1 = c1; h2 = s_c1; h3 = p_c2; h4 = c2; h5 = p_c3; h6 = c3;
				    goto exchange;
				}
			      
				/* Or perform the 3-opt move where no subtour inversion is necessary 
				   i.e. delete edges (c1,s_c1), (c2,p_c2), (c3,s_c3) and 
				   add edges (c1,c2), (c3,s_c1), (p_c2,s_c3) */
			      
				gain = - radius - (*m_Vdistance)[p_c2][c2] - 
				    (*m_Vdistance)[c3][s_c3]
				    + add1 + add2 + (*m_Vdistance)[p_c2][s_c3];
			      
				/* check for improvement */
				if ( gain < move_value ) {
				    improvement_flag = TRUE;
				    move_value = gain;
				    opt2_flag = FALSE;
				    move_flag = 4;
				    improvement_flag = TRUE;
				    /* store nodes involved in move */
				    h1 = c1; h2 = s_c1; h3 = p_c2; h4 = c2; h5 = c3; h6 = s_c3; 
				    goto exchange;
				}
			    }
			}
			else
			    g = nn_ls + 1;
		    }
		    g++;
		}
		h++;
	    }
	    if ( move_flag || opt2_flag ) {
	    exchange:
		move_value = 0;

		/* Now make the exchange */
		if ( move_flag ) {
		    dlb[h1] = FALSE; dlb[h2] = FALSE; dlb[h3] = FALSE; 
		    dlb[h4] = FALSE; dlb[h5] = FALSE; dlb[h6] = FALSE;
		    pos_c1 = pos[h1]; pos_c2 = pos[h3]; pos_c3 = pos[h5];
		  
		    if ( move_flag == 4 ) {

			if ( pos_c2 > pos_c1 ) 
			    n1 = pos_c2 - pos_c1;
			else
			    n1 = n - (pos_c1 - pos_c2);
			if ( pos_c3 > pos_c2 ) 
			    n2 = pos_c3 - pos_c2;
			else
			    n2 = n - (pos_c2 - pos_c3);
			if ( pos_c1 > pos_c3 ) 
			    n3 = pos_c1 - pos_c3;
			else
			    n3 = n - (pos_c3 - pos_c1);
		      
			/* n1: length h2 - h3, n2: length h4 - h5, n3: length h6 - h1 */
			val[0] = n1; val[1] = n2; val[2] = n3; 
			/* Now order the partial tours */
			h = 0;
			help = LONG_MIN;
			for ( g = 0; g <= 2; g++) {
			    if ( help < val[g] ) {
				help = val[g];
				h = g;
			    }
			}
		      
			/* order partial tours according length */
			if ( h == 0 ) {
			    /* copy part from pos[h4] to pos[h5]
			       direkt kopiert: Teil von pos[h6] to pos[h1], it
			       remains the part from pos[h2] to pos[h3] */
			    j = pos[h4];
			    h = pos[h5];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h) {
				i++;
				j++;
				if ( j  >= n )
				    j = 0;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    /* First copy partial tour 3 in new position */
			    j = pos[h4];
			    i = pos[h6];
			    tour[j] = tour[i];
			    pos[tour[i]] = j; 
			    while ( i != pos_c1) {
				i++;
				if ( i >= n )
				    i = 0;
				j++;
				if ( j >= n )
				    j = 0;
				tour[j] = tour[i];
				pos[tour[i]] = j; 
			    }
			  
			    /* Now copy stored part from h_tour */
			    j++;
			    if ( j >= n )
				j = 0;
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
			else if ( h == 1 ) {
			  
			    /* copy part from pos[h6] to pos[h1]
			       direkt kopiert: Teil von pos[h2] to pos[h3], it
			       remains the part from pos[h4] to pos[h5] */
			    j = pos[h6];
			    h = pos[h1];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h) {
				i++;
				j++;
				if ( j  >= n )
				    j = 0;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    /* First copy partial tour 3 in new position */
			    j = pos[h6];
			    i = pos[h2];
			    tour[j] = tour[i];
			    pos[tour[i]] = j; 
			    while ( i != pos_c2) {
				i++;
				if ( i >= n )
				    i = 0;
				j++;
				if ( j >= n )
				    j = 0;
				tour[j] = tour[i];
				pos[tour[i]] = j; 
			    }
			  
			    /* Now copy stored part from h_tour */
			    j++;
			    if ( j >= n )
				j = 0;
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
			else if ( h == 2 ) {
			    /* copy part from pos[h2] to pos[h3]
			       direkt kopiert: Teil von pos[h4] to pos[h5], it
			       remains the part from pos[h6] to pos[h1] */
			    j = pos[h2];
			    h = pos[h3];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h) {
				i++;
				j++;
				if ( j  >= n )
				    j = 0;
				h_tour[i] = tour[j];
				n1++;
			    }
	      
			    /* First copy partial tour 3 in new position */
			    j = pos[h2];
			    i = pos[h4];
			    tour[j] = tour[i];
			    pos[tour[i]] = j; 
			    while ( i != pos_c3) {
				i++;
				if ( i >= n )
				    i = 0;
				j++;
				if ( j >= n )
				    j = 0;
				tour[j] = tour[i];
				pos[tour[i]] = j; 
			    }
			  
			    /* Now copy stored part from h_tour */
			    j++;
			    if ( j >= n )
				j = 0;
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i]; 
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		    }
		    else if ( move_flag == 1 ) {
		      
			if ( pos_c3 < pos_c2 ) 
			    n1 = pos_c2 - pos_c3;
			else
			    n1 = n - (pos_c3 - pos_c2);
			if ( pos_c3 > pos_c1 ) 
			    n2 = pos_c3 - pos_c1 + 1;
			else
			    n2 = n - (pos_c1 - pos_c3 + 1);
			if ( pos_c2 > pos_c1 ) 
			    n3 = n - (pos_c2 - pos_c1 + 1);
			else
			    n3 = pos_c1 - pos_c2 + 1;
		      
			/* n1: length h6 - h3, n2: length h5 - h2, n2: length h1 - h3 */
			val[0] = n1; val[1] = n2; val[2] = n3; 
			/* Now order the partial tours */
			h = 0;
			help = LONG_MIN;
			for ( g = 0; g <= 2; g++) {
			    if ( help < val[g] ) {
				help = val[g];
				h = g;
			    }
			}
			/* order partial tours according length */
		      
			if ( h == 0 ) {
			  
			    /* copy part from pos[h5] to pos[h2]
			       (inverted) and from pos[h4] to pos[h1] (inverted)
			       it remains the part from pos[h6] to pos[h3] */
			    j = pos[h5];
			    h = pos[h2];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    j = pos[h1];
			    h = pos[h4];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				hh_tour[i] = tour[j];
				n2++;
			    }
			  
			    j = pos[h4];
			    for ( i = 0; i< n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if (j >= n)
				    j = 0;
			    }
			  
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i< n1 ; i++ ) {
				tour[j] = h_tour[i]; 
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
			else if ( h == 1 ) {
			  
			    /* copy part from h3 to h6 (wird inverted) erstellen : */
			    j = pos[h3];
			    h = pos[h6];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h) {
				i++;
				j--;
				if ( j  < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    j = pos[h6];
			    i = pos[h4];
			  
			    tour[j] = tour[i];
			    pos[tour[i]] = j; 
			    while ( i != pos_c1) {
				i++;
				j++;
				if ( j >= n)
				    j = 0;
				if ( i >= n)
				    i = 0;
				tour[j] = tour[i];
				pos[tour[i]] = j; 
			    }
			  
			    /* Now copy stored part from h_tour */
			    j++;
			    if ( j >= n )
				j = 0;
			    i = 0;
			    tour[j] = h_tour[i];
			    pos[h_tour[i]] = j; 
			    while ( j != pos_c1 ) {
				j++;
				if ( j >= n )
				    j = 0;
				i++;
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
			    }
			    tour[n] = tour[0];
			}
		      
			else if ( h == 2 ) {
			  
			    /* copy part from pos[h2] to pos[h5] and
			       from pos[h3] to pos[h6] (inverted), it
			       remains the part from pos[h4] to pos[h1] */
			    j = pos[h2];
			    h = pos[h5];
			    i = 0;
			    h_tour[i] =  tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j++;
				if ( j >= n )
				    j = 0;
				h_tour[i] = tour[j];
				n1++;
			    }
			    j = pos_c2;
			    h = pos[h6];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				hh_tour[i] = tour[j];
				n2++;
			    }
			  
			    j = pos[h2];
			    for ( i = 0; i< n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n)
				    j = 0;
			    }
			  
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i< n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		    }
		    else if ( move_flag == 2 ) {
		      
			if ( pos_c3 < pos_c1 ) 
			    n1 = pos_c1 - pos_c3;
			else
			    n1 = n - (pos_c3 - pos_c1);
			if ( pos_c3 > pos_c2 ) 
			    n2 = pos_c3 - pos_c2;
			else
			    n2 = n - (pos_c2 - pos_c3);
			if ( pos_c2 > pos_c1 ) 
			    n3 = pos_c2 - pos_c1;
			else
			    n3 = n - (pos_c1 - pos_c2);
		      
			val[0] = n1; val[1] = n2; val[2] = n3; 
			/* Determine which is the longest part */
			h = 0;
			help = LONG_MIN;
			for ( g = 0; g <= 2; g++) {
			    if ( help < val[g] ) {
				help = val[g];
				h = g;
			    }
			}
			/* order partial tours according length */
		      
			if ( h == 0 ) {
			  
			    /* copy part from pos[h3] to pos[h2]
			       (inverted) and from pos[h5] to pos[h4], it
			       remains the part from pos[h6] to pos[h1] */
			    j = pos[h3];
			    h = pos[h2];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    j = pos[h5];
			    h = pos[h4];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				hh_tour[i] = tour[j];
				n2++;
			    }
			  
			    j = pos[h2];
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i]; 
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
	      
			    for ( i = 0; i < n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			    /*  	      getchar(); */
			}
			else if ( h == 1 ) {
			  
			    /* copy part from pos[h2] to pos[h3] and
			       from pos[h1] to pos[h6] (inverted), it
			       remains the part from pos[h4] to pos[h5] */
			    j = pos[h2];
			    h = pos[h3];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j++;
				if ( j >= n  )
				    j = 0;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    j = pos[h1];
			    h = pos[h6];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j =  n-1;
				hh_tour[i] = tour[j];
				n2++;
			    }
			    j = pos[h6];
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i]; 
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    for ( i = 0; i < n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		      
			else if ( h == 2 ) {
			  
			    /* copy part from pos[h1] to pos[h6]
			       (inverted) and from pos[h4] to pos[h5],
			       it remains the part from pos[h2] to
			       pos[h3] */
			    j = pos[h1];
			    h = pos[h6];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }

			    j = pos[h4];
			    h = pos[h5];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h ) {
				i++;
				j++;
				if ( j >= n  )
				    j = 0;
				hh_tour[i] = tour[j];
				n2++;
			    }

			    j = pos[h4];
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			  
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i < n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		    }
		    else if ( move_flag == 3 ) {
		      
			if ( pos_c3 < pos_c1 ) 
			    n1 = pos_c1 - pos_c3;
			else
			    n1 = n - (pos_c3 - pos_c1);
			if ( pos_c3 > pos_c2 ) 
			    n2 = pos_c3 - pos_c2;
			else
			    n2 = n - (pos_c2 - pos_c3);
			if ( pos_c2 > pos_c1 ) 
			    n3 = pos_c2 - pos_c1;
			else
			    n3 = n - (pos_c1 - pos_c2);
			/* n1: length h6 - h1, n2: length h4 - h5, n2: length h2 - h3 */
		      
			val[0] = n1; val[1] = n2; val[2] = n3; 
			/* Determine which is the longest part */
			h = 0;
			help = LONG_MIN;
			for ( g = 0; g <= 2; g++) {
			    if ( help < val[g] ) {
				help = val[g];
				h = g;
			    }
			}
			/* order partial tours according length */
		      
			if ( h == 0 ) {
			  
			    /* copy part from pos[h2] to pos[h3]
			       (inverted) and from pos[h4] to pos[h5]
			       it remains the part from pos[h6] to pos[h1] */
			    j = pos[h3];
			    h = pos[h2];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }
			  
			    j = pos[h2];
			    h = pos[h5];
			    i = pos[h4];
			    tour[j] = h4;
			    pos[h4] = j;
			    while ( i != h ) {
				i++;
				if ( i >= n )
				    i = 0;
				j++;
				if ( j >= n )
				    j = 0;
				tour[j] = tour[i];
				pos[tour[i]] = j;
			    }
			    j++;
			    if ( j >= n )
				j = 0;
			    for ( i = 0; i < n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
			else if ( h == 1 ) {

			    /* copy part from pos[h3] to pos[h2]
			       (inverted) and from  pos[h6] to pos[h1],
			       it remains the part from pos[h4] to pos[h5] */
			    j = pos[h3];
			    h = pos[h2];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0  )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }

			    j = pos[h6];
			    h = pos[h1];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h ) {
				i++;
				j++;
				if ( j >= n )
				    j = 0;
				hh_tour[i] = tour[j];
				n2++;
			    }
			  
			    j = pos[h6];
			    for ( i = 0; i<n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }

			    for ( i = 0 ; i < n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		      
			else if ( h == 2 ) {
			  
			    /* copy part from pos[h4] to pos[h5]
			       (inverted) and from pos[h6] to pos[h1] (inverted)
			       it remains the part from pos[h2] to pos[h3] */
			    j = pos[h5];
			    h = pos[h4];
			    i = 0;
			    h_tour[i] = tour[j];
			    n1 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				h_tour[i] = tour[j];
				n1++;
			    }

			    j = pos[h1];
			    h = pos[h6];
			    i = 0;
			    hh_tour[i] = tour[j];
			    n2 = 1;
			    while ( j != h ) {
				i++;
				j--;
				if ( j < 0 )
				    j = n-1;
				hh_tour[i] = tour[j];
				n2++;
			    }

			    j = pos[h4];
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i< n1 ; i++ ) {
				tour[j] = h_tour[i];
				pos[h_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    /* Now copy stored part from h_tour */
			    for ( i = 0; i< n2 ; i++ ) {
				tour[j] = hh_tour[i];
				pos[hh_tour[i]] = j; 
				j++;
				if ( j >= n )
				    j = 0;
			    }
			    tour[n] = tour[0];
			}
		    }
		    else {
			printf(" Some very strange error must have occurred !!!\n\n");
			exit(0);
		    }
		}
		if (opt2_flag) {

		    /* Now perform move */
		    dlb[h1] = FALSE; dlb[h2] = FALSE;
		    dlb[h3] = FALSE; dlb[h4] = FALSE;
		    if ( pos[h3] < pos[h1] ) {
			help = h1; h1 = h3; h3 = help;
			help = h2; h2 = h4; h4 = help;
		    }
		    if ( pos[h3]-pos[h2] < n / 2 + 1) {
			/* reverse inner part from pos[h2] to pos[h3] */
			i = pos[h2]; j = pos[h3];
			while (i < j) {
			    c1 = tour[i];
			    c2 = tour[j];
			    tour[i] = c2;
			    tour[j] = c1;
			    pos[c1] = j;
			    pos[c2] = i;
			    i++; j--;
			}
		    }
		    else {
			/* reverse outer part from pos[h4] to pos[h1] */
			i = pos[h1]; j = pos[h4];
			if ( j > i )
			    help = n - (j - i) + 1;
			else 
			    help = (i - j) + 1;
			help = help / 2;
			for ( h = 0 ; h < help ; h++ ) {
			    c1 = tour[i];
			    c2 = tour[j];
			    tour[i] = c2;
			    tour[j] = c1;
			    pos[c1] = j;
			    pos[c2] = i;
			    i--; j++;
			    if ( i < 0 )
				i = n - 1;
			    if ( j >= n )
				j = 0;
			}
			tour[n] = tour[0];
		    }
		}
	    }
	    else {
		dlb[c1] = TRUE;
	    }
	}
    }
   
    free( h_tour );
    free( hh_tour );
    free( pos );
    free( dlb );

    }





	~CLocalSearch(void)
	{
	}
};

