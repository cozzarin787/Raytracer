#pragma once
#include "IntersectData.h"
#include <string>

class Material
{
public:

	float kr;
	float kt;
	float indexRefract;

	Material();
	Material(float kr, float kt);
	Material(float kr, float kt, float ni);


	virtual Color illuminate(IntersectData interData) = 0;
	virtual std::string toString() = 0;

	~Material();
};

