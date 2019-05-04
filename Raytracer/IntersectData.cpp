#include "IntersectData.h"

IntersectData::IntersectData(Point p, RowVector3f normal, std::vector<RowVector3f> lightDirs, RowVector3f view,
        std::vector<LightSource> lightList, Color ambientLight, float ni, float nt, bool inside)
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
    // inside -> outside
    if(inside)
    {
        this->T = refract(this->N, Ray(this->P, this->V), nt, ni);
    }
    // outside -> inside
    else
    {
        this->T = refract(this->N, Ray(this->P, this->V), ni, nt);
    }


}

Ray IntersectData::reflect(RowVector3f normal, Ray r)
{
	RowVector3f direction = (r.direction - (2 * normal * (r.direction.dot(normal)))).normalized();
	return Ray(r.origin, direction);
}
Ray IntersectData::refract(RowVector3f normal, Ray r, float ni, float nt)
{
    if(ni == nt)
    {
		this->totalInternalRefraction = false;
        return r;
    }
    else
    {
        RowVector3f direction = ni * ( r.direction - normal * (r.direction.dot(normal))) / nt;
        float internalReflection = ( 1 - std::pow(ni, 2) * (1 - std::pow(r.direction.dot(normal), 2) )) / std::pow(nt, 2);

        if(internalReflection < 0)
        {
            this->totalInternalRefraction = true;
            return reflect(-1 * normal, r);
        }
        else {
            this->totalInternalRefraction = false;
            return Ray(r.origin, direction + (-1 * normal) * std::sqrt(internalReflection));
        }
    }
}

IntersectData::~IntersectData()
{
}


