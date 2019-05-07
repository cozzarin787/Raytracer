#include "IntersectData.h"

IntersectData::IntersectData(Point p, RowVector3f normal, std::vector<RowVector3f> lightDirs, RowVector3f view,
        std::vector<LightSource> lightList, Color ambientLight, float ni, float nt)
{
	this->P = p;							
	this->N = normal;						
	this->S = lightDirs;	
	this->ambientLight = ambientLight;
	this->V = view;
	this->lights = lightList;

	// Calculate Reflection Rays
	for (int i = 0; i < this->S.size(); i++)
	{
		this->R.push_back(reflect(this->N, Ray(this->P, -1 * this->S[i])));
	}

	// Calculate Halfway Vectors to implement PhongBlinn
	for (int i = 0; i < this->S.size(); i++)
	{
		this->H.push_back((this->V + this->S[i]).normalized());
	}

	// Calculate Transmission Ray
	Ray r = Ray(this->P, this->V);
    // inside -> outside
    if(r.direction.dot(this->N) < 0)
    {
        this->T = refract(-1 * this->N, r, nt, ni);
    }
    // outside -> inside
    else
    {
        this->T = refract(this->N, r, ni, nt);
    }
}

Ray IntersectData::reflect(Vector3f normal, Ray r)
{
	Vector3f direction = (r.direction - (2 * normal * (r.direction.dot(normal)))).normalized();
	return Ray(r.origin, direction);
}

Ray IntersectData::refract(const RowVector3f normal, Ray r, float ni, float nt)
{
    if(ni == nt)
    {
        return r;
    }
    else
    {
        RowVector3f direction = ni * ( r.direction -  normal * (r.direction.dot(normal))) / nt;
        float internalReflection = ( 1 - std::powf(ni, 2) * (1 - std::powf(r.direction.dot(normal), 2) )) / std::powf(nt, 2);

        if(internalReflection < 0)
        {
            return reflect(normal, r);
        }
        else {
            return Ray(r.origin, direction + normal * std::sqrtf(internalReflection));
        }
    }
}

IntersectData::~IntersectData()
{
}


