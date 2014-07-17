#include "StdAfx.h"
#include "TSPalgorithm.h"



#include "resource.h"

#include "CMinMaxAntSystem.h"
#include "CElitistAntSystem.h"
#include "CGeneticSystem.h"
#include "CAntColonySystem.h"
#include "BestWorstAntSystem.h"
#include "CAntSystem.h"

UINT MyThreadMMAS(LPVOID pParam)
{
	
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	// get ticks per second
	QueryPerformanceFrequency(&Frequency); 
	
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	std::vector<std::vector<int> > Matrix = tsp->matrix_;
	Parameters par= tsp->GetParameters();
	CMinMaxAntSystem mmas(par , &Matrix);

	int d =(std::numeric_limits<int>::max)();
	mmas.initPheromones();
	mmas.calculateHeuristicMatrix();
	mmas.initAnts();

	for(int i = 0; i < par.Epochs; i++)
	{
		QueryPerformanceCounter(&StartingTime);
		mmas.constructSolutions();
		mmas.localSearch();
		mmas.updatePheromones();
		mmas.incIteration();
		QueryPerformanceCounter(&EndingTime);

	

		int dist= (int)mmas.getBestPathLengthSofar();

		if(dist < d)
		{ 
		
			d = dist;
			static newTour tour;
			tour.Tour = mmas.getBestSoFarPath();
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
		//	tsp->Notify( tour );
			
			//post message to window
		}
		else if(tsp->m_EndThread)
		{
			break;
		}
		
		else if(i%25 == 0)
		{
			ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
			ElapsedMicroseconds.QuadPart *= 1000000;
			ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
			_Longlong l= ElapsedMicroseconds.QuadPart;


			AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );
		
			std::string varAsString = std::to_string(l);
			std::string s="time for epoch" + varAsString ;
			static std::string c;
			c=s;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		}

	}
	tsp->m_pThread =0;
	return 0;
}

UINT MyThreadElite(LPVOID pParam)
{
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	std::vector<std::vector<int> > matrix = tsp->matrix_;


	Parameters par= tsp->GetParameters();

	CElitistAntSystem mmas(par,&matrix );
	int d =(std::numeric_limits<int>::max)();

	//int oDistance = m_Soltions[file];

	mmas.initPheromones();
	mmas.calculateHeuristicMatrix();
	mmas.initAnts();
	for(int i = 0; i < par.Epochs; i++)
	{
		mmas.constructSolutions();
		mmas.localSearch();
		mmas.updatePheromones();
		mmas.incIteration();


		int dist= (int)mmas.getBestPathLengthSofar();

		if(dist < d && (i%20 ==0))
		{ 
			d = dist;
			static newTour tour;
			tour.Tour = mmas.getBestSoFarPath();
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
			//	tsp->Notify( tour );

			//post message to window
		}
		if(tsp->m_EndThread)
		{
			break;
		}

		AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );

		static std::string s="test\r\n";

		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);

	}
	tsp->m_pThread =0;
	return 0;
}

UINT MyThreadACS(LPVOID pParam)
{
	
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	long double elapsedTime;

	// get ticks per second
	QueryPerformanceFrequency(&frequency);

	
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	Parameters par= tsp->GetParameters();

	std::vector<std::vector<int> > matrix = tsp->matrix_;

	size_t num_cities =matrix.size();


	int third = 1;
	
	//double ro=0.1 , xi=0.1 , q0=0.9;
	//int ants =(int) (num_cities/10);

	CAntColonySystem mmas(par,&matrix);
	int d =(std::numeric_limits<int>::max)();

	//int oDistance = m_Soltions[file];

	mmas.initPheromones();
	mmas.calculateHeuristicMatrix();
	mmas.initAnts();
	for(int i = 0; i < par.Epochs; i++)
	{
		QueryPerformanceCounter(&t1);
		mmas.constructSolutions();
		mmas.localSearch();
		mmas.updatePheromones();
		mmas.incIteration();
		QueryPerformanceCounter(&t2);


		int dist= (int)mmas.getBestPathLengthSofar();

		if(dist < d)
		{ 
			d = dist;
			static newTour tour;
			tour.Tour = mmas.getBestSoFarPath();
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
			//	tsp->Notify( tour );

			//post message to window
		}
		if(tsp->m_EndThread)
		{
			break;
		}
		if(i%25 == 0)
		{
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );
			std::string varAsString = std::to_string(elapsedTime);
			static std::string s="time for epoch" + varAsString ;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		}
		

	}
	tsp->m_pThread =0;
	return 0;
}
UINT MyThreadGEN(LPVOID pParam)
{
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	Parameters par= tsp->GetParameters();
	std::vector<std::vector<int> > matrix = tsp->matrix_;
	int d =(std::numeric_limits<int>::max)();

	size_t num_cities =matrix.size();

	int ants = static_cast<int>(num_cities);
	double mutationRatio = 0.9;
	CGeneticSystem ga(ants , num_cities ,mutationRatio, false , false , &matrix);

	ga.initPopulation();

	for(int i = 0; i < 5000; i++)
	{
		ga.stepGeneration();
		ga.localSearch();
		ga.setIteration(ga.getIteration()+1);
		int dist= (int)ga.computePathLength(ga.getBestSoFarPath());
		
		
		if(dist < d)
		{
			d = dist;
			static newTour tour;
		//	tour.Tour= reinterpret_cast<size_t>(ga.getBestSoFarPath());
			//tour.Tour = ga
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
		}
		if(tsp->m_EndThread)
		{
			break;
		}
		AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );

		static std::string s="test\r\n";

		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
	}
		
	return 0;
}

UINT MyThreadAS(LPVOID pParam)
{
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	Parameters par= tsp->GetParameters();
	std::vector<std::vector<int> > matrix = tsp->matrix_;

	//int num_cities =matrix.size();

//	int ants = num_cities;
//	int third = 1;

	CAntSystem mmas(par, &matrix);
	int d =(std::numeric_limits<int>::max)();


	mmas.initPheromones();
	mmas.calculateHeuristicMatrix();
	mmas.initAnts();
	for(int i = 0; i < par.Epochs; i++)
	{
		mmas.constructSolutions();
		mmas.localSearch();
		mmas.updatePheromones();
		mmas.incIteration();


		int dist= (int)mmas.getBestPathLengthSofar();

		if(dist < d)
		{ 
			d = dist;
			static newTour tour;
			tour.Tour = mmas.getBestSoFarPath();
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
			//	tsp->Notify( tour );

			//post message to window
		}
		if(tsp->m_EndThread)
		{
			break;
		}

		AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );

		//static std::string s="test\r\n";

		//AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);

	}
	tsp->m_pThread =0;
	return 0;
}

UINT MyThreadBWA(LPVOID pParam)
{
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	Parameters par= tsp->GetParameters();
	std::vector<std::vector<int> > matrix = tsp->matrix_;

	//int num_cities =matrix.size();

//	int ants = num_cities;
//	int third = 1;

	CBestWorstAntSystem mmas(par , &matrix);
	int d =(std::numeric_limits<int>::max)();


	mmas.initPheromones(); //set to ramdom
	mmas.calculateHeuristicMatrix(); //cal table
	mmas.initAnts();  //clear ants
	for(int i = 0; i < par.Epochs; i++)
	{
		mmas.constructSolutions();
		mmas.localSearch();
		mmas.updatePheromones();
		mmas.incIteration();


		int dist= (int)mmas.getBestPathLengthSofar();

		if(dist < d)
		{ 
			d = dist;
			static newTour tour;
			tour.Tour = mmas.getBestSoFarPath();
			tour.Irreation = i;
			tour.TourLenght = d;

			AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
			//	tsp->Notify( tour );

			//post message to window
		}
		if(tsp->m_EndThread)
		{
			break;
		}

		if(i> 259)
		{
				AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );

		}

		AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)i );

	//	static std::string s="test\r\n";

	//	AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);

	}
	tsp->m_pThread =0;
	return 0;
}


TSPalgorithm::TSPalgorithm(void)
{
	createSolutionsMap();
	m_pThread = 0;
	m_EndThread = false;
}


TSPalgorithm::~TSPalgorithm(void)
{
}

void TSPalgorithm::createSolutionsMap()
{
	populateSolutionMap(string("berlin52") , 7542);
	populateSolutionMap(string("eil51") , 426);
	populateSolutionMap(string("pr2392") , 378032);
	populateSolutionMap(string("ulysses16") , 6859);
	populateSolutionMap(string("fl1400") , 20127);
	populateSolutionMap(string("rat783") , 8806);
	populateSolutionMap(string("pr2392") , 378032);

}

// Run the optimization algorithm
void TSPalgorithm::Run(Parameters &par ,   std::vector<std::vector<int> >  &matrix )
{

	this->m_par =par;

	if(!m_pThread)
	{
		//enum {MMAS = 0 , ACS=1 , AS=2 , GS=3, EAS=4 , BWAS=5};
		matrix_ = matrix;
		m_EndThread = false;

		switch (par.ALG)
		{

		case MMAS: {
					m_pThread = AfxBeginThread(MyThreadMMAS,((LPVOID)this));
					break;
				}
		case ACS: {
			m_pThread = AfxBeginThread(MyThreadACS,((LPVOID)this));
			break;
				}
		case AS: {
			m_pThread = AfxBeginThread(MyThreadAS,((LPVOID)this));
			break;
				}
		case GS: {
			m_pThread = AfxBeginThread(MyThreadGEN,((LPVOID)this));
			break;
				}
		case EAS: {
			m_pThread = AfxBeginThread(MyThreadMMAS,((LPVOID)this));
			break;
				}
		case BWAS: {
			m_pThread = AfxBeginThread(MyThreadBWA,((LPVOID)this));
			break;
				}
		default:
			m_pThread = AfxBeginThread(MyThreadMMAS,((LPVOID)this));
			break;
		}


	//	m_pThread = AfxBeginThread(MyThreadMMAS,((LPVOID)this));
		//MyThreadElite
	//	m_pThread = AfxBeginThread(MyThreadElite,((LPVOID)this));
		//MyThreadBWA
	//	m_pThread = AfxBeginThread(MyThreadBWA,((LPVOID)this));
		//MyThreadAS
//		m_pThread = AfxBeginThread(MyThreadAS,((LPVOID)this));
		//MyThreadGEN
	//	m_pThread = AfxBeginThread(MyThreadGEN,((LPVOID)this));
		//MyThreadACS
		//m_pThread = AfxBeginThread(MyThreadACS,((LPVOID)this));
	}

}



// Notify observers of changes
/*
void TSPalgorithm::Notify( newTour tour )  
{ 
	bestTour = tour;

    for ( std::vector<Observer*>::iterator it = ob_set.begin();  
          it != ob_set.end();  
          it++ )  
    {  
		Observer* ob = *it;  
	//	PostMessage()

//		ob->UpdateDisplay( tour );  
    }  
}  
*/

// Add observer
//void TSPalgorithm::AddObserver( Observer* ob )
//{
//	ob_set.push_back( ob );
//}
// Get the stored city
//int TSPalgorithm::GetStoredTourCity( const int& index )
//{
//	return bestTour.Tour[index];
//}