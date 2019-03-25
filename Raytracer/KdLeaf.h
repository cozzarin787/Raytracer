#pragma once
#include <vector>
#include "Object.h"
#include "KdNode.h"
#include "World.h"

class KdLeaf : public KdNode
{
public:
	std::vector<World::voxelObjectWrapper> primitives;

	KdLeaf();
	KdLeaf(std::vector<World::voxelObjectWrapper> p);

	std::string toString();

	~KdLeaf();
};

