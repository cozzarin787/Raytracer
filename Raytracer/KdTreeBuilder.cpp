#include "KdTreeBuilder.h"

constexpr auto X_AXIS = 0;
constexpr auto Y_AXIS = 1;
constexpr auto Z_AXIS = 2;

KdTreeBuilder::KdTreeBuilder()
{
}

Plane KdTreeBuilder::getPartitionPlane(Voxel v)
{
	// Alternate axis
	this->i++;

	RowVector3f normal;
	switch (this->i % 3)
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

	RowVector3f spatialMedian = (v.max.vector() + v.min.vector()) / 2.0f;
	Point planePoint = Point(spatialMedian[0], spatialMedian[1], spatialMedian[2]);
	return Plane(planePoint, normal);
}

std::vector<World::voxelObjectWrapper> KdTreeBuilder::getObjectsInVoxel(Voxel v, std::vector<World::voxelObjectWrapper> primitives)
{
	std::vector<World::voxelObjectWrapper> newList;
	for (int i = 0; i < primitives.size(); i++)
	{
		if (primitives[i].v.inVoxel(v))
		{
			newList.push_back(primitives[i]);
		}
	}
	return newList;
}

KdNode* KdTreeBuilder::getNode(Voxel v, std::vector<World::voxelObjectWrapper> primitives)
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
		Voxel v1 = v;
		Voxel v2 = v;
		float seperateValue;

		switch (this->i % 3)
		{
			case X_AXIS:
				seperateValue = (v1.max.x + v1.min.x) / 2.0f;
				v1.max.x = seperateValue;
				v2.min.x = seperateValue;
				break;
			case Y_AXIS:
				seperateValue = (v1.max.y + v1.min.y) / 2.0f;
				v1.min.y = seperateValue;
				v2.max.y = seperateValue;
				break;
			case Z_AXIS:
				seperateValue = (v1.max.z + v1.min.z) / 2.0f;
				v1.max.z = seperateValue;
				v2.min.z = seperateValue;
				break;
		}

		// Split list of objects based on the partitioning plane
		std::vector<World::voxelObjectWrapper> L_1 = getObjectsInVoxel(v1, primitives);
		std::vector<World::voxelObjectWrapper> L_2 = getObjectsInVoxel(v2, primitives);

		KdInterior node = KdInterior(P, (i % 3), v, getNode(v1, L_1), getNode(v2, L_2));
		KdNode* retNode = &node;
		return retNode;
	}
}

/*
	intersect(KDTree Node N, Ray R)
		if (N is a leaf):
			go though elements in N and return closest.
		else:
			a = where R enters the voxel (changing coordinate)
			b = where R leaves the voxel (changing coordinate)
			S = seperating plane
			
*/

std::vector<Object::intersectResult> KdTreeBuilder::rayThroughTree(KdNode* N, Ray r)
{
	std::vector<Object::intersectResult> retVal;

	if (dynamic_cast<KdLeaf*>(N))
	{
		KdLeaf* leaf = dynamic_cast<KdLeaf*>(N);

		for (int i = 0; i < leaf->primitives.size(); i++)
		{
			Object::intersectResult ir = leaf->primitives[i].o->intersect(r);
			if (ir.isIntersection)
			{
				retVal.push_back(ir);
			}
		}
		return retVal;
	}
	else
	{
		KdInterior* node = dynamic_cast<KdInterior*>(N);
		Voxel::intersectVoxel interData = node->v.intersect(r);
		float a;
		float b;
		float P;

		// Check to see what is the changing coordinate between the voxel and seperating plane
		// take the coordinate that changes which is all is need for case analysis
		switch (node->changingAxis)
		{
			case X_AXIS:
				a = interData.enter.x;
				b = interData.exit.x;
				P = node->s_plane.p.x;
				break;
			case Y_AXIS:
				a = interData.enter.y;
				b = interData.exit.y;
				P = node->s_plane.p.y;
				break;
			case Z_AXIS:
				a = interData.enter.z;
				b = interData.exit.z;
				P = node->s_plane.p.z;
				break;
		}
		// case 1: Only crosses node1 voxel (a and b above P)
		if (a >= P && b >= P)
			rayThroughTree(node->node1, r);
		// case 2: Only crosses node2 voxel (a and b below P)
		else if (a < P && b < P)
			rayThroughTree(node->node2, r);
		// case 3: Starts node1, goes to node2 (P between a and b, where a > b)
		else if (a > b)
		{
			std::vector<Object::intersectResult> intersection = rayThroughTree(node->node1, r);
			if (intersection.empty())
				rayThroughTree(node->node2, r);
		}
		// case 4: Starts node2, goes to node1 (P between a and b, where b > a)
		else if (b > a)
		{
			std::vector<Object::intersectResult> intersection = rayThroughTree(node->node2, r);
			if (intersection.empty())
				rayThroughTree(node->node1, r);
		}
	}
}



KdTreeBuilder::~KdTreeBuilder()
{
}
