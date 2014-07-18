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

	   void opt3(std::vector<size_t> &tour){

	//	tour.resize(tour.size()+1);
	//	tour[tour.size()-1]= tour[0];
	


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
		//tour.resize(tour.size()-1);

    }
    void swapV(std::vector<size_t> &tour , size_t i, size_t j)
	{
        size_t swap = tour[i];
        tour[i] = tour[j];
        tour[j] = swap;
    }

	std::vector<int> greedyPath(int noNodes)
	{
        std::vector<bool> visted(noNodes);
		std::vector<int> tour(noNodes+1);
		
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
	//   tour[noNodes] = tour[0];
       return tour;
   }
	void greedyAntPath(std::vector<size_t> &tour)
	{
		auto noNodes = tour.size();
		std::vector<bool> visted(noNodes);
			
	    int min, node;
        int i,j;

        for(i = 0; i < noNodes; i++)
            visted[i] = false;

       // tour[0] = 0;
     
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
      //tour[noNodes] = tour[0];
   }

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
		size_t N = tour.size()-1;
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


	~CLocalSearch(void)
	{
	}
};

