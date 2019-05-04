#include "TRReinhard.h"

#define epsilon 0.0000001f

TRReinhard::TRReinhard(float Ld_max)
{
	this->Ld_max = Ld_max;
}

void TRReinhard::reproduceTone(std::vector<std::vector<Color>> &pixelIntensities)
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

	for (int i = 0; i < pixelIntensities.size(); i++)
	{
		for (int j = 0; j < pixelIntensities[i].size(); j++)
		{
			// Create luminance values by mapping key value to zone V (18% gray)
			pixelIntensities[i][j].r *= (0.18f / avgLogLum);
			pixelIntensities[i][j].g *= (0.18f / avgLogLum);
			pixelIntensities[i][j].b *= (0.18f / avgLogLum);

			// Find the reflectance based on film-like response
			pixelIntensities[i][j].r = (pixelIntensities[i][j].r / (1.0f + pixelIntensities[i][j].r));
			pixelIntensities[i][j].g = (pixelIntensities[i][j].g / (1.0f + pixelIntensities[i][j].g));
			pixelIntensities[i][j].b = (pixelIntensities[i][j].b / (1.0f + pixelIntensities[i][j].b));

			// Calculate target display luminance by simulating illumination
			pixelIntensities[i][j].r *= this->Ld_max;
			pixelIntensities[i][j].g *= this->Ld_max;
			pixelIntensities[i][j].b *= this->Ld_max;

			// Get RGB 0-255 value
			pixelIntensities[i][j].r = (pixelIntensities[i][j].r / this->Ld_max) * 255;
			pixelIntensities[i][j].g = (pixelIntensities[i][j].g / this->Ld_max) * 255;
			pixelIntensities[i][j].b = (pixelIntensities[i][j].b / this->Ld_max) * 255;
		}
	}
}


TRReinhard::~TRReinhard()
{
}
