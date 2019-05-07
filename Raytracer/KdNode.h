#pragma once
#include "Point.h"
class KdNode
{
public:
	KdNode();

	virtual std::string toString() = 0;

	~KdNode();
};

