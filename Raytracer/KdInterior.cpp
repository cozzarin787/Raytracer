#include "KdInterior.h"

KdInterior::KdInterior(Plane p, unique_ptr<KdNode> node1, unique_ptr<KdNode> node2) : KdNode()
{
	this->plane = p;
}

KdInterior::~KdInterior()
{
}
