#include "StdAfx.h"
#include "CAnt.h"


CAnt::CAnt(size_t n)
	: m_antTour(n+1) , m_visted(n) , tourLength(0)
{
	//this->m_antTour.resize(n);
//	this->m_visted.resize(n);
    tourLength = 0;
 //   noNodes = n;

}
CAnt::~CAnt(void)
{
}
