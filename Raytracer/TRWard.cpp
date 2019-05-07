#include "TRWard.h"
#include <math.h>
#include <iostream>

#define epsilon 0.0000001f
#define print(x) std::cout << x << std::endl;

TRWard::TRWard(float Ld_max) : TROperator()
{
	this->Ld_max = Ld_max;
}

void TRWard::reproduceTone(std::vector<std::vector<Color>> &pixelIntensities)
{
	// Find average Log luminance of the pixels
	int halfI = (int)(pixelIntensities.size() / 2);
	int halfJ = (int)(pixelIntensities[0].size() / 2);
	float L_wa = 0.27f * pixelIntensities[halfI][halfJ].r + 0.67f * pixelIntensities[halfI][halfJ].g + 0.06f * pixelIntensities[halfI][halfJ].b;

	// Calculate sf using Ward's TR Operator
	float n = (1.219f + powf((this->Ld_max / 4.0f), 0.4f));
	float d = (1.219f + powf(L_wa, 0.4f));
	float sf = powf((n / d), 2.5f);

	// Apply sf to each RGB value of each pixel
	for (int i = 0; i < pixelIntensities.size(); i++)
	{
		for (int j = 0; j < pixelIntensities[i].size(); j++)
		{
			pixelIntensities[i][j].r = (pixelIntensities[i][j].r * sf) * (1.0f / this->Ld_max) * 255.0f;
			pixelIntensities[i][j].g = (pixelIntensities[i][j].g * sf) * (1.0f / this->Ld_max) * 255.0f;
			pixelIntensities[i][j].b = (pixelIntensities[i][j].b * sf) * (1.0f / this->Ld_max) * 255.0f;
		}
	}
}


TRWard::~TRWard()
{
}
