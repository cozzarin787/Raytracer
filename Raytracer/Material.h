#pragma once
#include "IntersectData.h"
#include <string>

class Material
{
public:

	float kr;
	float kt;

	Material();
	Material(float kr, float kt);


	virtual Color illuminate(IntersectData interData) = 0;
	virtual std::string toString() = 0;

	~Material();
};

