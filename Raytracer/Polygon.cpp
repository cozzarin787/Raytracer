#include "Polygon.h"
#define PI 3.141592654f

Polygon::Polygon(Material* mat, std::vector<Point> v_list) : Object(mat)
{
	this->vertices = v_list;
	
	// calc normal
	Point p0 = vertices[0];
	Point p1 = vertices[1];
	Point p2 = vertices[2];
	RowVector3f v1 = p1.vector() - p0.vector();
	RowVector3f v2 = p2.vector() - p0.vector();
	this->normal = (v1.cross(v2).normalized());
	this->F = -this->normal[0] * p0.x - this->normal[1] * p0.y - this->normal[2] * p0.z;
}

Object::intersectResult Polygon::intersect(Ray r)
{
	float denom = this->normal[0] * r.direction[0] + this->normal[1] * r.direction[1] + this->normal[2] * r.direction[2];
	if (denom == 0)
	{
		return intersectResult(false);
	}

	float numan = -1 * (this->normal[0] * r.origin.x + this->normal[1] * r.origin.y + this->normal[2] * r.origin.z + this->F);
	float omega = numan / denom;
	if (omega <= epsilon)
	{
		return intersectResult(false);
	}

	// calc intersection point
	float xi = r.origin.x + r.direction[0] * omega;
	float yi = r.origin.y + r.direction[1] * omega;
	float zi = r.origin.z + r.direction[2] * omega;
	Point inter = Point(xi, yi, zi);

	// check if point of intersection lies within the boundaries of the polygon
	RowVector3f A = RowVector3f(this->vertices[this->vertices.size() - 1].vector() - inter.vector()).normalized();
	RowVector3f B = RowVector3f(this->vertices[0].vector() - inter.vector()).normalized();
	float angleSum = acos(A.dot(B) / (A.norm() * B.norm())) * (180.0f / PI);

	for (int i = 0; i < this->vertices.size()-1; i++)
	{
		A = RowVector3f(this->vertices[i].vector() - inter.vector()).normalized();
		B = RowVector3f(this->vertices[i+1].vector() - inter.vector()).normalized();
		angleSum += acos((A.dot(B))) * (180.0f / PI);
	}

	if (angleSum >= 359.5f && angleSum <= 360.5f)
	{
		return intersectResult(true, omega, this->mat, inter, this->normal);
	}
	else
	{
		return intersectResult(false);
	}
}

void Polygon::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform points of the polygon
	for (int i = 0; i < this->vertices.size(); i++)
	{
		RowVector4f pHomo = this->vertices[i].homogen();
		RowVector4f pPrimeHomo = transMat * pHomo.transpose();
		float w = pPrimeHomo[3];
		this->vertices[i] = Point(pPrimeHomo[0] / w, pPrimeHomo[1] / w, pPrimeHomo[2] / w);
	}

	// Recalculate normal
	Point p0 = vertices[0];
	Point p1 = vertices[1];
	Point p2 = vertices[2];
	RowVector3f v1 = p1.vector() - p0.vector();
	RowVector3f v2 = p2.vector() - p0.vector();
	this->normal = (v1.cross(v2).normalized());

	// Recalculate F
	this->F = -this->normal[0] * this->vertices[0].x - this->normal[1] * this->vertices[0].y - this->normal[2] * this->vertices[0].z;
}

std::string Polygon::toString()
{	
	std::string v_list = "";
	for (int i = 0; i < this->vertices.size(); i++)
	{
		std::string str_i = std::to_string(i);

		std::string x = std::to_string(this->vertices[i].x);
		std::string y = std::to_string(this->vertices[i].y);
		std::string z = std::to_string(this->vertices[i].z);
		v_list += "   p" + str_i + "- x: " + x + " y: " + y + " z: " + z + "\n";
	}
	std::string x = std::to_string(this->normal[0]);
	std::string y = std::to_string(this->normal[1]);
	std::string z = std::to_string(this->normal[2]);
	std::string m = this->mat->toString();
	return std::string("Polygon\n List of Vertices: \n" + v_list + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Polygon::~Polygon()
{
}
