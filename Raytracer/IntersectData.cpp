#include "IntersectData.h"

IntersectData::IntersectData(Point p, RowVector3f normal, RowVector3f lightDir, RowVector3f view, std::vector<LightSource> lightList)
{
	this->P = p;							
	this->N = normal;						
	this->S = lightDir;	
	this->V = view;
	// Calculate Reflection Vector
	this->R = this->S - 2 * (this->S.dot(this->N) / this->N.squaredNorm()) * this->N;
	this->lights = lightList;	
}

IntersectData::~IntersectData()
{
}
