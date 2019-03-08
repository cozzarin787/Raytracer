#pragma once
#include "KdNode.h"
#include "Plane.h"
#include "Voxel.h"
#include <memory>

using std::unique_ptr;

class KdInterior : public KdNode
{
public:
	Plane s_plane;
	Voxel v1;
	Voxel v2;

	unique_ptr<KdNode> node1;
	unique_ptr<KdNode> node2;

	KdInterior();
	KdInterior(Plane p, Voxel v1, Voxel v2, unique_ptr<KdNode> node1, unique_ptr<KdNode> node2);

	~KdInterior();
};

