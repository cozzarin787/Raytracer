#pragma once
#include "LightSource.h"
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix;
using Eigen::RowMajor;

class World
{
public:
	struct voxelObjectWrapper
	{
		Object* o;
		Voxel v;

		voxelObjectWrapper(Object* object, Voxel voxel)
		{
			this->o = object;
			this->v = voxel;
		};
	};
	
	std::vector<voxelObjectWrapper> voxelObjectList;
	std::vector<Object*> objectList;
	std::vector<LightSource> lightList;
	Color background;

	World();
	World(Color background);

	int add(Object* o);
	int addLight(LightSource light);
	void transform(int index, Matrix<float, 4, 4, RowMajor> transMat);
	void transformAllObjects(Matrix<float, 4, 4, RowMajor> transMat);
	void transformAllLights(Matrix<float, 4, 4, RowMajor> transMat);
	std::vector<Object::intersectResult> spawnRay(Ray r);

	std::string toString();

	~World();
};

