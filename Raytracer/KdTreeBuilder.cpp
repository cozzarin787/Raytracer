#include "KdInterior.h"
#include "KdLeaf.h"

constexpr auto X_AXIS = 0;
constexpr auto Y_AXIS = 1;
constexpr auto Z_AXIS = 2;

int i = 0;

Plane getPartitionPlane(Voxel v)
{
	// Alternate axis
	RowVector3f normal;
	switch (i % 3)
	{
		case X_AXIS:
			normal = RowVector3f(1, 0, 0);
			break;
		case Y_AXIS:
			normal = RowVector3f(0, 1, 0);
			break;
		case Z_AXIS:
			normal = RowVector3f(0, 0, 1);
			break;
		default:
			normal = RowVector3f(1, 0, 0);
			break;
	}
	i++;

	RowVector3f spatialMedian = (v.max.vector() + v.min.vector()) / 2.0f;
	Point planePoint = Point(spatialMedian.x, spatialMedian.y, spatialMedian.z);
	return Plane(planePoint, normal);
}

std::vector<Object*> getObjectsInVoxel(Voxel v, std::vector<Object*> primitives)
{
	std::vector<Object*> newList;
	for (int i = 0; i < primitives.size(); i++)
	{
		if (primitives[i]->inVoxel(v))
		{
			newList.push_back(primitives[i]);
		}
	}
	return newList;
}

KdNode* getNode(Voxel v, std::vector<Object*> primitives)
{
	if (primitives.size() <= 1)
	{
		KdLeaf node = KdLeaf(primitives);
		KdNode* retNode = &node;
		return retNode;
	}
	else
	{
		// Create a partitioning plane
		Plane P = getPartitionPlane(v);

		// Create new voxels based on partitioning plane
		Voxel v1;
		Voxel v2;

		// Split list of objects based on the partitioning plane
		std::vector<Object*> L_1 = getObjectsInVoxel(v1, primitives);
		std::vector<Object*> L_2 = getObjectsInVoxel(v2, primitives);

		KdNode* top = getNode(v1, L_1);
		KdNode* bottom = getNode(v2, L_2);
		KdInterior node = KdInterior(P, v, top, bottom);
		KdNode* retNode = &node;
		return retNode;
	}
}
