#pragma once
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix4f;

class World
{
public:
	std::vector<Object> objectList;

	World();

	int add(Object o);
	void transform(int index, Matrix4f transMat);
	void transformAllObjects(Matrix4f transMat);
	std::vector<Object::intersectResult> spawnRay(Ray r);

	std::string toString();

	~World();
};

