#include "Voxel.h"

Voxel::Voxel()
{
}

Voxel::Voxel(Point min, Point max)
{
	this->min = min;
	this->max = max;
	this->bounds[0] = this->min;
	this->bounds[1] = this->max;
}

Voxel::intersectVoxel Voxel::intersect(Ray r)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[r.invSign[0]].x - r.origin.x) * r.invDir[0];
	tmax = (bounds[1 - r.invSign[0]].x - r.origin.x) * r.invDir[0];
	tymin = (bounds[r.invSign[1]].y - r.origin.y) * r.invDir[1];
	tymax = (bounds[1 - r.invSign[1]].y - r.origin.y) * r.invDir[1];

	if ((tmin > tymax) || (tymin > tmax))
		return Voxel::intersectVoxel(false);
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[r.invSign[2]].z - r.origin.z) * r.invDir[2];
	tzmax = (bounds[1 - r.invSign[2]].z - r.origin.z) * r.invDir[2];

	if ((tmin > tzmax) || (tzmin > tmax))
		return Voxel::intersectVoxel(false);
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	// Ray intersects Voxel, return points a and b
	float xi, yi, zi;
	Point a, b;

	if (tmin < 0 && tmax < 0)
	{
		return Voxel::intersectVoxel(false);
	}
	else if (tmin < 0)
	{
		xi = r.origin.x + r.direction[0] * tmax;
		yi = r.origin.x + r.direction[1] * tmax;
		zi = r.origin.x + r.direction[2] * tmax;
		b = Point(xi, yi, zi);
		a = r.origin;
	}
	else if (tmax < 0)
	{
		xi = r.origin.x + r.direction[0] * tmin;
		yi = r.origin.x + r.direction[1] * tmin;
		zi = r.origin.x + r.direction[2] * tmin;
		b = Point(xi, yi, zi);
		a = r.origin;
	}
	else
	{
		xi = r.origin.x + r.direction[0] * tmax;
		yi = r.origin.x + r.direction[1] * tmax;
		zi = r.origin.x + r.direction[2] * tmax;
		b = Point(xi, yi, zi);
		xi = r.origin.x + r.direction[0] * tmin;
		yi = r.origin.x + r.direction[1] * tmin;
		zi = r.origin.x + r.direction[2] * tmin;
		a = Point(xi, yi, zi);
	}
	return Voxel::intersectVoxel(true, a, b);
}

bool Voxel::inVoxel(Voxel v)
{
	for (int i = 0; i < 3; i++)
	{
		if ((v.min.vector()[i] > this->max.vector()[i]) || (this->min.vector()[i] > v.max.vector()[i]))
		{
			return false;
		}
	}
	return true;
}

void Voxel::transform(Matrix4f transMat)
{
	// Transform min point
	Vector4f minHomo = this->min.homogen();
	Vector4f minPrimeHomo = transMat * minHomo;
	float w = minPrimeHomo[3];
	this->min = Point(minPrimeHomo[0] / w, minPrimeHomo[1] / w, minPrimeHomo[2] / w);

	// Transform max point
	Vector4f maxHomo = this->max.homogen();
	Vector4f maxPrimeHomo = transMat * maxHomo;
	w = maxPrimeHomo[3];
	this->max = Point(maxPrimeHomo[0] / w, maxPrimeHomo[1] / w, maxPrimeHomo[2] / w);

	// Transform bounds
	for (int i = 0; i < 2; i++)
	{
		Vector4f boundHomo = this->bounds[i].homogen();
		Vector4f boundPrimeHomo = transMat * boundHomo;
		float w = boundPrimeHomo[3];
		this->bounds[i] = Point(boundPrimeHomo[0] / w, boundPrimeHomo[1] / w, boundPrimeHomo[2] / w);
	}
}

std::string Voxel::toString()
{
	return std::string("Voxel: \n min: " + min.toString() + "\n max:" + max.toString() + "\n");
}

Voxel::~Voxel()
{
}
