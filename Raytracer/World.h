#pragma once
#include "LightSource.h"
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix4f;

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
	
	Voxel totalBound;
	std::vector<voxelObjectWrapper> voxelObjectList;
	std::vector<Object*> objectList;
	std::vector<LightSource*> lightList;
	Color background;
	float indexRefract;

	World();
	World(Color background);
    World(Color background, float indexRefract);

	int add(Object* o);
	int addLight(LightSource* light);
	void transform(int index, Matrix4f transMat);
	void transformAllObjects(Matrix4f transMat);
	void transformAllLights(Matrix4f transMat);
	void calcWorldVoxel();
	std::vector<Object::intersectResult> spawnRay(Ray r);

	std::string toString();

	~World();
};

