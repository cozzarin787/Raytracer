#include "KdInterior.h"

KdInterior::KdInterior()
{
}

KdInterior::KdInterior(Plane p, int ca, Voxel v, KdNode* node1, KdNode* node2) : KdNode()
{
	this->changingAxis = ca;
	this->s_plane = Plane(p.p, p.normal);
	this->v = v;
	this->node1 = node1;
	this->node2 = node2;
}

std::string KdInterior::toString()
{
	std::string s1 = this->s_plane.toString();
	std::string s2 = this->v.toString();
	return std::string("InteriorNode: \n Seperating Plane: " + s1 + "\n Voxel: " + s2 + "\n");
}

KdInterior::~KdInterior()
{
}
