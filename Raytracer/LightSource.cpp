#include "LightSource.h"

LightSource::LightSource(Point position, Color c)
{
	this->position = position;
	this->color = c;
}

void LightSource::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform center of sphere
	RowVector4f centerHomo = this->position.homogen();
	RowVector4f centerPrimeHomo = transMat * centerHomo.transpose();
	float w = centerPrimeHomo[3];
	this->position = Point(centerPrimeHomo[0] / w, centerPrimeHomo[1] / w, centerPrimeHomo[2] / w);
}

LightSource::~LightSource()
{
}
