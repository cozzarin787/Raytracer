#include "Voxel.h"



Voxel::Voxel()
{
}

Voxel::Voxel(Point min, Point max)
{
	this->min = min;
	this->max = max;
}

Voxel::intersectVoxel Voxel::intersect(Ray r)
{
	return Voxel::intersectVoxel(Point(0,0,0), Point(0,0,0));
}

bool Voxel::inVoxel(Voxel v)
{
	for (int i = 0; i < 3; i++)
	{
		if ((this->min.vector()[i] > v.max.vector()[i]) || (v.min.vector()[i] > this->max.vector()[i]))
		{
			return false;
		}

		return true;
	}
}

void Voxel::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform min point
	RowVector4f minHomo = this->min.homogen();
	RowVector4f minPrimeHomo = transMat * minHomo.transpose();
	float w = minPrimeHomo[3];
	this->min = Point(minPrimeHomo[0] / w, minPrimeHomo[1] / w, minPrimeHomo[2] / w);

	// Transform max point
	RowVector4f maxHomo = this->max.homogen();
	RowVector4f maxPrimeHomo = transMat * maxHomo.transpose();
	w = maxPrimeHomo[3];
	this->max = Point(maxPrimeHomo[0] / w, maxPrimeHomo[1] / w, maxPrimeHomo[2] / w);
}

std::string Voxel::toString()
{
	return std::string("Voxel: \n min: " + min.toString() + "\n max:" + max.toString() + "\n");
}

Voxel::~Voxel()
{
}
