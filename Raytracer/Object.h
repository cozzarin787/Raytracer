#pragma once
#include "Material.h"
#include "Ray.h"

using Eigen::Matrix;
using Eigen::RowMajor;

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
	virtual void transform(Matrix<float, 4, 4, RowMajor> transMat) = 0;

	virtual std::string toString() = 0;

	virtual ~Object();
};

