#include "TROperator.h"
#pragma once

class TRWard : public TROperator
{
public:
	float Ld_max;

	TRWard(float Ld_max);

	void reproduceTone(std::vector<std::vector<Color>> &pixelIntensities);

	~TRWard();
};

