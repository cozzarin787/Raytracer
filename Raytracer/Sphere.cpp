#include "Sphere.h"

Sphere::Sphere(Material mat, Point center, float radius) : Object(mat)
{
	this->center = center;
	this->radius = radius;
	this->radius_sqr = radius*radius;
};

Object::intersectResult Sphere::intersect(Ray r)
{
	//Origin of sphere
	float xc = this->center.x;
	float yc = this->center.y;
	float zc = this->center.z;

	//Origin of ray
	float x0 = r.origin.x;
	float y0 = r.origin.y;
	float z0 = r.origin.z;

	//Direction
	float dx = r.direction[0];
	float dy = r.direction[1];
	float dz = r.direction[2];
	
	float A = 1;
	float B = 2 * (dx*(x0 - xc) + dy*(y0 - yc) + dz*(z0 - zc));
	float C = pow((x0 - xc), 2) + pow((y0 - yc), 2) + pow((z0 - zc), 2) - this->radius_sqr;

	float roots = pow(B,2)-4*A*C;
	float omega;
	float xi, yi, zi;
	
	Point intersectPoints[2];
	RowVector3f normals[2];

	if (roots < 0)
	{
		// no real root, no intersection
		return intersectResult(false);
	}
	else if (roots == 0) 
	{ 
		// one root, ray intersects at sphere's surface
		omega = (-B + sqrt(roots)) / 2;
		xi = x0 + dx * omega;
		yi = y0 + dy * omega;
		zi = z0 + dz * omega;
		intersectPoints[0] = Point(xi, yi, zi);
	}
	else
	{
		// two roots, ray goes through sphere
		for (int i = 0; i < 2; i++) 
		{
			if (i == 0)
			{
				omega = (-B + sqrt(roots)) / 2;
			}
			else
			{
				float temp = (-B - sqrt(roots)) / 2;
				if (temp < omega)
				{
					omega = temp;
				}
			}
			xi = x0 + dx * omega;
			yi = y0 + dy * omega;
			zi = z0 + dz * omega;
			intersectPoints[i] = Point(xi, yi, zi);
		}
	}
	for (Point p : intersectPoints)
	{
		int i = 0;
		normals[i++] = RowVector3f(xi - xc, yi - yc, zi - zc).normalized();
	}
	
	return intersectResult(true, omega, this->mat);
}

void Sphere::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform center of sphere
	RowVector4f centerHomo = this->center.homogen();
	RowVector4f centerPrimeHomo = centerHomo * transMat;
	float w = centerPrimeHomo[3];
	this->center = Point(centerPrimeHomo[0] / w, centerPrimeHomo[1] / w, centerPrimeHomo[2] / w);
}

std::string Sphere::toString()
{
	std::string m = this->mat.toString();
	std::string c = this->center.toString();
	std::string r = std::to_string(this->radius);
	return std::string("Sphere\n Material: " + m + "\n Center: " + c + "\n Radius: " + r);
}

Sphere::~Sphere()
{
}
