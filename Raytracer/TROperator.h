#include "Color.h"
#include <vector>
#pragma once

class TROperator
{
public:
	TROperator();

	virtual void reproduceTone(std::vector<std::vector<Color>> &pixelIntensities) = 0;

	~TROperator();
};

