#pragma once
#include "Material.h"

class Object
{
public:

	Material mat;

	Object(Material mat);

	virtual float intersect(Ray r) = 0;

	virtual std::string toString() = 0;

	virtual ~Object();
};

