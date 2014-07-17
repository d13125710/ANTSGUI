#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <array> //for std::array


#include "Tokenizer.h"
#include "TokenFinder.h"
#include "DistanceHelper.h"

using namespace std;
typedef Tokenizer<TokenFinder> Tk;
typedef std::vector<std::vector<int> > MatrixArrayType;

class Coords{
public:
	double x;
	double y;

	double GetX(){
		return x;
	}
	double GetY()
	{
		return y;
	}
};

class CTSPLIBFileReader
{
	
private:
	//open
	//bool openFileStream(const string &str);
	void calculateDistanceMatrix();

private: 
	std::ifstream m_inFileStream; 
	std::string m_fileName;
	string m_sInput;
	vector<Coords> m_coordsVector;
 	MatrixArrayType m_theMatrix;
	IDistance *p_mDistanceCalculator;

public:
	bool Read();
	double ShowSolution();
	double ReadMatrix(int x, int y) const{
		return  m_theMatrix[x][y];
	}
	MatrixArrayType CopyMatrix(){
		return m_theMatrix; 
	}
	vector<Coords> CopyCoordsMatrix(){
		return m_coordsVector;
	}

	int getMatrixSize() const {
	     return  (int)m_theMatrix.size();
	}
	CTSPLIBFileReader(const string &str);
	~CTSPLIBFileReader(void); 

	int minx;
	int miny;
	int maxx;
	int maxy;


};

