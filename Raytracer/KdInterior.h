#pragma once
#include "KdNode.h"
#include "Plane.h"
#include <memory>

class KdInterior : public KdNode
{
public:
	Plane plane;
	std::unique_ptr<KdNode> node1;
	std::unique_ptr<KdNode> node2;

	KdInterior();
	~KdInterior();
};

