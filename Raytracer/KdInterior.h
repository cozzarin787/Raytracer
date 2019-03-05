#pragma once
#include "KdNode.h"
#include "Plane.h"
#include <memory>

using std::unique_ptr;

class KdInterior : public KdNode
{
public:
	Plane plane;
	unique_ptr<KdNode> node1;
	unique_ptr<KdNode> node2;

	KdInterior(Plane p, unique_ptr<KdNode> node1, unique_ptr<KdNode> node2);
	~KdInterior();
};

