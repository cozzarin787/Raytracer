#pragma once
#include <vector>
#include "Object.h"
#include "KdNode.h"

class KdLeaf : public KdNode
{
public:
	std::vector<Object*> primitives;

	KdLeaf();
	KdLeaf(std::vector<Object*> p);

	~KdLeaf();
};

