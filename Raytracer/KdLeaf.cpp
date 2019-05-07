#include "KdLeaf.h"

KdLeaf::KdLeaf()
{
}

KdLeaf::KdLeaf(std::vector<World::voxelObjectWrapper> p) : KdNode()
{
	this->primitives = p;
}

std::string KdLeaf::toString()
{
	std::string s;
	for (int i = 0; i < this->primitives.size(); i++)
	{
		s += "Voxel: " + this->primitives[i].v.toString() + "\nObject: " + this->primitives[i].o->toString() + "\n";
	}

	return std::string("KdLeaf: \nPrimitive List: \n" + s);
}

KdLeaf::~KdLeaf()
{
}
