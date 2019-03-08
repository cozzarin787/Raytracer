#include "KdInterior.h"

KdInterior::KdInterior()
{
}

KdInterior::KdInterior(Plane p, Voxel v1, Voxel v2, unique_ptr<KdNode> node1, unique_ptr<KdNode> node2) : KdNode()
{
	this->s_plane = Plane(p.p, p.normal);
	this->v1 = v1;
	this->v2 = v2;
}

KdInterior::~KdInterior()
{
}
