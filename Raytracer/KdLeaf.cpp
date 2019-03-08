#include "KdLeaf.h"

KdLeaf::KdLeaf()
{
}

KdLeaf::KdLeaf(std::vector<Object*> p) : KdNode()
{
	this->primitives = p;
}

KdLeaf::~KdLeaf()
{
}
