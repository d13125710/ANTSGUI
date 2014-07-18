#pragma once

#include <vector>


class CAnt
{
	std::vector<size_t> m_antTour;
    std::vector<bool> m_visted;

	//int noNodes;
    int tourLength;
   
public:
	int getAntTourLength() const
	{
        return tourLength;
    }
	void setAntTourLength(int length)
	{
		tourLength=length;
	}
	void setAntsTour(std::vector<size_t> &antTour)
	{
		m_antTour=antTour;
	}
	bool isCityVisited(size_t city) const
	{
        return m_visted[city];
    }
	void setCityVisited(size_t city)
	{
        m_visted[city]=true;
    }
	void ReinitiseAnt()
	{
		tourLength =0;
		for(unsigned int i=0;i < m_visted.size();i++)
		{
				m_antTour[i]=0;
				m_visted[i]=false;
		}
	

	}
	size_t getCity(size_t index) const {
        return m_antTour[index];
    }
	std::vector<size_t>& getAntsCityTour(){
        return m_antTour;
    }
	void setAntCity(size_t index, size_t city)
	{
		//make sure city not already there
		m_antTour[index] = city;
    }
	size_t getNoNodes() const {
		return m_antTour.size();
	}
	
	
	CAnt(size_t noOfCitys);
	CAnt(){}
	~CAnt(void);
};

