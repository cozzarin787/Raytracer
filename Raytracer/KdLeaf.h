#pragma once
#include <vector>
#include "Object.h"

class KdLeaf
{
public:
	std::vector<Object*> primitives;

	KdLeaf(std::vector<Object*> p);

	~KdLeaf();
};

