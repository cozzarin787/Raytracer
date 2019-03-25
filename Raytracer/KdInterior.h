#pragma once
#include "KdNode.h"
#include "Plane.h"
#include "Voxel.h"
#include <memory>

using std::unique_ptr;

class KdInterior : public KdNode
{
public:
	int changingAxis;

	Plane s_plane;
	Voxel v;

	KdNode* node1;
	KdNode* node2;

	KdInterior();
	KdInterior(Plane p, int changingAxis, Voxel v, KdNode* node1, KdNode* node2);
	std::string toString();

	~KdInterior();
};

