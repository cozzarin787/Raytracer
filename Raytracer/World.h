#pragma once
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix;
using Eigen::RowMajor;

class World
{
public:
	std::vector<Object*> objectList;

	World();

	int add(Object* o);
	void transform(int index, Matrix<float, 4, 4, RowMajor> transMat);
	void transformAllObjects(Matrix<float, 4, 4, RowMajor> transMat);
	std::vector<Object::intersectResult> spawnRay(Ray r);

	std::string toString();

	~World();
};

