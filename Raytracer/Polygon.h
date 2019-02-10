#pragma once
#include "Object.h"
#include "Ray.h"
#include <vector>

class Polygon : public Object
{
public:
	std::vector<Point> vertices;
	RowVector3f normal;

	Polygon(Material mat, std::vector<Point> v_list);
	
	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);

	std::string toString();

	~Polygon();
};

