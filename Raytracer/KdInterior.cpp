#include "KdInterior.h"

KdInterior::KdInterior()
{
}

KdInterior::KdInterior(Plane p, Voxel v, KdNode * node1, KdNode * node2)
{
}

KdInterior::KdInterior(Plane p, Voxel v, KdNode* node1, KdNode* node2) : KdNode()
{
	this->s_plane = Plane(p.p, p.normal);
	this->v = v;
	this->node1 = node1;
	this->node2 = node2;
}

KdInterior::~KdInterior()
{
}
