#pragma once
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix4f;

class Polygon : public Object
{
public:
	std::vector<Point> vertices;
	Vector3f normal;
	float F;

	Polygon(Material* mat, std::vector<Point> v_list);
	
	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);
	bool inVoxel(Voxel v);

	std::string toString();

	~Polygon();
};

