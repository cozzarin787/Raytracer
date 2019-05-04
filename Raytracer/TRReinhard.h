#include "TROperator.h"
#pragma once
class TRReinhard : public TROperator
{
public:
	float Ld_max;

	TRReinhard(float Ld_max);

	void reproduceTone(std::vector<std::vector<Color>> &pixelIntensities);

	~TRReinhard();
};

