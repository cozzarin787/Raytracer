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
	float avgLogLum = 0.0f;

	for (int i = 0; i < pixelIntensities.size(); i++)
	{
		for (int j = 0; j < pixelIntensities[i].size(); j++)
		{
			// convert pixel luminance to scene luminance
			float totalLum = 0.27f * pixelIntensities[i][j].r + 0.67f * pixelIntensities[i][j].g + 0.06f * pixelIntensities[i][j].b;
			
			// Calculate log luminance
			avgLogLum += log10f(totalLum + epsilon);
		}
	}

	avgLogLum /= (float)(pixelIntensities.size() * pixelIntensities[0].size());
	avgLogLum = powf(10.0f, avgLogLum);

	// Calculate sf using Ward's TR Operator
	float n = (1.219f + powf((this->Ld_max / 2.0f), 0.4f));
	float d = (1.219f + powf(avgLogLum, 0.4f));
	float sf = (1.0f / this->Ld_max) * powf((n / d), 2.5f);
	print(sf);
	// Apply sf to each RGB value of each pixel
	for (int i = 0; i < pixelIntensities.size(); i++)
	{
		for (int j = 0; j < pixelIntensities[i].size(); j++)
		{
			pixelIntensities[i][j].r = (pixelIntensities[i][j].r * sf) * 255.0f;
			pixelIntensities[i][j].g = (pixelIntensities[i][j].g * sf) * 255.0f;
			pixelIntensities[i][j].b = (pixelIntensities[i][j].b * sf) * 255.0f;
		}
	}
}


TRWard::~TRWard()
{
}
