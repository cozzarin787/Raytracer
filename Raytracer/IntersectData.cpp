#include "IntersectData.h"

IntersectData::IntersectData(Point p, RowVector3f normal, std::vector<RowVector3f> lightDirs, RowVector3f view, std::vector<LightSource> lightList, Color ambientLight)
{
	this->P = p;							
	this->N = normal;						
	this->S = lightDirs;	
	this->ambientLight = ambientLight;
	this->V = view;
	// Calculate Reflection Vectors
	for (int i = 0; i < this->S.size(); i++)
	{
		this->R.push_back(reflect(this->N, Ray(this->P, -1 * this->S[i])).direction);
	}
	// Calculate Halfway Vectors
	// TODO
	this->lights = lightList;	
}

Ray IntersectData::reflect(RowVector3f normal, Ray r)
{
	RowVector3f direction = r.direction - 2 * (r.direction.dot(normal) / normal.squaredNorm()) * normal;
	return Ray(r.origin, direction);
}

IntersectData::~IntersectData()
{
}
