#pragma once
#include "Material.h"

class Object
{
public:

	struct intersectResult
	{
		bool isIntersection;
		float omega;
		Material mat;

		intersectResult()
		{};

		intersectResult(bool i) 
		{
			isIntersection = i;
		};

		intersectResult(bool i, float o, Material m)
		{
			isIntersection = i;
			omega = o;
			mat = m;
		};
	};

	Material mat;

	Object(Material mat);

	virtual intersectResult intersect(Ray r) = 0;

	virtual std::string toString() = 0;

	virtual ~Object();
};

