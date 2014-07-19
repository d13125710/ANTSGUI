#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils.h"




namespace antlogger {

class CLog
{
	//std::ofstream fout;
	std::ofstream m_inFileStream; 

public:
	CLog( const char *filename , Parameters &p)
	{
		m_inFileStream.open(filename);	
		if (m_inFileStream.is_open()) {
			m_inFileStream << "Ants System " << p.lambda << std::endl;
			m_inFileStream << "Log file created" << std::endl << std::endl;
		}
	}
	~CLog(void){
		if (m_inFileStream.is_open()) 
		{
			m_inFileStream.flush();
			m_inFileStream.close();	
		}
	}
	friend CLog &operator << (CLog &logger, const char *text) {

		logger.m_inFileStream << text << std::endl;
		return logger;

	}
	//clog << std::string("test data\n");
	friend CLog &operator << (CLog &logger, std::string *text) {

		logger.m_inFileStream << *text << std::endl;
		return logger;

	}
	friend CLog &operator << (CLog &logger, std::vector<size_t> &data) {

		for(size_t i=0; i< data.size();i++)
			logger.m_inFileStream << data[i] << ",";
		logger.m_inFileStream <<std::endl;
		return logger;

	
	}
		template<typename T>
	void logMatrix(std::vector<std::vector<T> > *matrix)
	{
		m_inFileStream << ("Matrix:\n");
		for (size_t i = 0 ; i < matrix->size() ; i++) {
			m_inFileStream << ("From :") << i << "\t" ;
			for ( size_t j = 0 ; j < matrix->size() - 1 ; j++ ) {
				 m_inFileStream <<  (*matrix)[i][j] << ",";
			}
			m_inFileStream << (*matrix)[i][ matrix->size() - 1] << std::endl;

		}
		m_inFileStream <<  std::endl; 

	}


};
}
