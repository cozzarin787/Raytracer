#pragma once
#include "IntersectData.h"
#include <string>

class Material
{
public:
	Material();

	virtual Color illuminate(IntersectData interData) = 0;
	virtual std::string toString() = 0;

	~Material();
};

