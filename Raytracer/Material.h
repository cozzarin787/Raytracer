#pragma once
#include "Object.h"
#include "IntersectData.h"
#include <string>

class Material
{
public:
	Material();

	virtual RowVector3f illuminate(IntersectData interData) = 0;
	virtual std::string toString() = 0;

	~Material();
};

