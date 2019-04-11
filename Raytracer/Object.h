#pragma once
#include "Material.h"
#include "Ray.h"
#include "Voxel.h"

#define epsilon 0.001f

using Eigen::Matrix4f;

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
		Vector3f normal;

		intersectResult()
		{};

		intersectResult(bool i) 
		{
			isIntersection = i;
		};

		intersectResult(bool i, float o, Material* m, Point p, Vector3f v)
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
	virtual void transform(Matrix4f transMat) = 0;
	virtual bool inVoxel(Voxel v) = 0;

	virtual std::string toString() = 0;

	~Object();
};

