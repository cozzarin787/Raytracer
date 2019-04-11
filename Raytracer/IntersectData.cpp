#include "IntersectData.h"

IntersectData::IntersectData(Point p, Vector3f normal, std::vector<Vector3f> lightDirs, Vector3f view, std::vector<LightSource*> lightList, Color ambientLight)
{
	this->P = p;							
	this->N = normal;						
	this->S = lightDirs;	
	this->ambientLight = ambientLight;
	this->V = view;
	// Calculate Reflection Vectors
	for (int i = 0; i < this->S.size(); i++)
	{
		this->R.push_back(reflect(this->N, Ray(this->P, this->S[i])).direction);
	}
	// Calculate Halfway Vectors
	// TODO
	this->lights = lightList;
}

Ray IntersectData::reflect(Vector3f normal, Ray r)
{
	Vector3f direction = (r.direction - (2 * normal * (r.direction.dot(normal)))).normalized();
	return Ray(r.origin, direction);
}

IntersectData::~IntersectData()
{
}
