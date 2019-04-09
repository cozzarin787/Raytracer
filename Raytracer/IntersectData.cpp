#include "IntersectData.h"

IntersectData::IntersectData(Point p, RowVector3f normal, std::vector<RowVector3f> lightDirs, RowVector3f view, std::vector<LightSource> lightList, Color ambientLight)
{
	this->P = p;							
	this->N = normal;						
	this->S = lightDirs;	
	this->ambientLight = ambientLight;
	this->V = view;
	// Calculate Reflection Rays
	for (int i = 0; i < this->S.size(); i++)
	{
		this->R.push_back(reflect(this->N, Ray(this->P, this->S[i])));
	}

	// Calculate Halfway Vectors to implement Phong-Blinn
	for (int i = 0; i < this->S.size(); i++)
	{
		this->H.push_back((this->V + this->S[i]).normalized());
	}
	this->lights = lightList;
}

Ray IntersectData::reflect(RowVector3f normal, Ray r)
{
	RowVector3f direction = (r.direction - (2 * normal * (r.direction.dot(normal)))).normalized();
	return Ray(r.origin, direction);
}

IntersectData::~IntersectData()
{
}
