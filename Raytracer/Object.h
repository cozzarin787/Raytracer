#pragma once
#include "Material.h"
#include "Ray.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Object
{
public:
	Material* mat;

	struct intersectResult
	{
		bool isIntersection;
		float omega;
		Material* mat;
		Point intersectPoint;
		RowVector3f normal;

		intersectResult()
		{};

		intersectResult(bool i) 
		{
			isIntersection = i;
		};

		intersectResult(bool i, float o, Material* m, Point p, RowVector3f v)
		{
			isIntersection = i;
			omega = o;
			mat = m;
			intersectPoint = p;
			normal = v;
		};
	};

	Object(Material* mat);

	virtual intersectResult intersect(Ray r) = 0;
	virtual void transform(Matrix<float, 4, 4, RowMajor> transMat) = 0;

	virtual std::string toString() = 0;

	~Object();
};

