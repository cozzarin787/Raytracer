#include "Polygon.h"

Polygon::Polygon(Material mat, std::vector<Point> v_list) : Object(mat)
{
	this->vertices = v_list;
	
	// calc normal
	Point p0 = vertices[0];
	Point p1 = vertices[1];
	Point p2 = vertices[2];
	RowVector3f v1 = p1.vector() - p0.vector();
	RowVector3f v2 = p2.vector() - p0.vector();
	this->normal = v1.cross(v2).normalized();
}

Object::intersectResult Polygon::intersect(Ray r)
{
	float denom = normal[0] * r.direction[0] - normal[1] * r.direction[1] - normal[2] * r.direction[2];
	if (denom <= 0)
	{
		return intersectResult(false);
	}

	float numan = normal[0] * r.origin.x - normal[1] * r.origin.y - normal[2] * r.origin.z;
	float omega = numan / denom;
	if (omega <= 0)
	{
		return intersectResult(false);
	}

	// calc intersection point
	float xi = r.origin.x + r.direction[0] * omega;
	float yi = r.origin.y + r.direction[1] * omega;
	float zi = r.origin.z + r.direction[2] * omega;
	Point inter = Point(xi, yi, zi);

	// check if point of intersection lies within the boundaries of the polygon
	RowVector3f A = RowVector3f(this->vertices[0].x - inter.x, this->vertices[0].y - inter.y, this->vertices[0].z - inter.z);
	RowVector3f B = RowVector3f(this->vertices[vertices.size() - 1].x - inter.x, this->vertices[vertices.size() - 1].y - inter.y, this->vertices[vertices.size() - 1].z - inter.z);
	float angleSum = acos((A.dot(B)) / (A.norm() * B.norm()));

	for (int i = 0; i < this->vertices.size()-1; i++)
	{
		RowVector3f A = RowVector3f(this->vertices[i].x - inter.x, this->vertices[i].y - inter.y, this->vertices[i].z - inter.z);
		RowVector3f B = RowVector3f(this->vertices[i+1].x - inter.x, this->vertices[i+1].y - inter.y, this->vertices[i+1].z - inter.z);
		angleSum += acos((A.dot(B)) / (A.norm() * B.norm()));
	}

	if (angleSum >= 359 && angleSum <= 361)
	{
		return intersectResult(true, omega, this->mat);
	}
	else
	{
		return intersectResult(false);
	}
}

void Polygon::transform(Matrix4f transMat)
{
	// Transform normal
	RowVector4f normHomo = RowVector4f(this->normal[0], this->normal[1], this->normal[2], 1);
	RowVector4f normPrimeHomo = normHomo * transMat;
	this->normal = RowVector3f(normPrimeHomo[0], normPrimeHomo[1], normPrimeHomo[2]).normalized();

	// Transform points of the polygon
	for (Point p : this->vertices)
	{
		RowVector4f pHomo = p.homogen();
		RowVector4f pPrimeHomo = pHomo * transMat;
		p = Point(pPrimeHomo[0], pPrimeHomo[1], pPrimeHomo[2]);
	}
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
	std::string m = this->mat.toString();
	return std::string("Polygon\n List of Vertices: \n" + v_list + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Polygon::~Polygon()
{
}
