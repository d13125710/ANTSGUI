#pragma once


#include <string>
#include <cstdarg>

#define IDC_INCITTERATION 2096
#define IDC_VERBOSE 2097
#define IDC_INCITTERATION 2098
#define ID_INDICATOR_NISH WM_USER +100
#define ID_INDICATOR_TIME WM_USER +101





class Parameters
{
public:
	enum {MMAS =5 , ACS=0 , AS=1 , GS=4, EAS=3 , BWAS=2 , RAS=6};
	int ALG; double rho;	int Alpha;	int Beta;	int noAnts;
	//al persefic eletist
	int EliteWeight;
	//ACS
	double xi; 	double q0; double r0;
	//search options
	bool LocalSearchOpt2;bool LocalSearchOpt3;bool LocalSearchGreed;
	//min max ant
	double lambda;
	//genetic
	double mutationRatio;
	int maxStagnationIterations;

	int Epochs;




	Parameters() : 
		EliteWeight(5) , xi(0.1) , q0(0.90) , r0(0.1) , LocalSearchOpt2(false) , LocalSearchOpt3(false) ,LocalSearchGreed(false) ,
			lambda(0.05) , mutationRatio(0.9) , maxStagnationIterations(100) , Epochs(5000)
	{
	
	}
	void setRho(int Q)
	{
		rho = (double) (0.1 * Q);
	}



};


//missing string printf
//this is safe and convenient but not exactly efficient
inline std::string format(const char* fmt, ...){
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if(size<=nsize){ //fail delete buffer and try again
		delete[] buffer;
		buffer = 0;
		buffer = new char[nsize+1]; //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}
