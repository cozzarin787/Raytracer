#include "Camera.h"

Camera::Camera(Point p, RowVector3f lookat, RowVector3f up)
{
	this->position = p;
	this->lookat = lookat;
	this->up = up;

	// construct view transform
	RowVector3f n = p.vector() - lookat.normalized();
	RowVector3f u = (up.cross(n)).normalized();
	RowVector3f v = n.cross(u);

	this->viewTransform.row(0) << u[0], u[1], u[2], -(p.vector().dot(u));
	this->viewTransform.row(1) << v[0], v[1], v[2], -(p.vector().dot(v));
	this->viewTransform.row(2) << n[0], n[1], n[2], -(p.vector().dot(n));
	this->viewTransform.row(3) << 0, 0, 0, 1;
}

void Camera::render(World world)
{
	// TODO
}

void Camera::setImageDim(int h, int w)
{
	this->imageHeightPx = h;
	this->imageWidthPx = w;
}

void Camera::setFilmPlaneDim(float h, float w)
{
	this->filmPlaneHeight = h;
	this->filmPlaneWidth = w;
}

void Camera::setFocalLength(float f)
{
	this->focalLength = f;
}

std::string Camera::toString()
{
	std::string result = "Camera\n Postion\n   " + this->position.toString() + "\n";
	std::string x = std::to_string(this->lookat[0]);
	std::string y = std::to_string(this->lookat[1]);
	std::string z = std::to_string(this->lookat[2]);
	result += "Lookat\n   [" + x + " " + y + " " + z + "]\n";
	x = std::to_string(this->up[0]);
	y = std::to_string(this->up[1]);
	z = std::to_string(this->up[2]);
	result += "Up\n   [" + x + " " + y + " " + z + "]\n";
	std::stringstream ss;
	ss << viewTransform;
	result += "View Transform\n   " + ss.str() + "\n";
	result += "Focal Length\n   " + std::to_string(focalLength) + "\n";
	result += "Image Dimenstions\n   HeightPx: " + std::to_string(imageHeightPx) + " WidthPx: " + std::to_string(imageWidthPx) + "\n";
	result += "Film Plane Dimenstions\n   HeightPx: " + std::to_string(filmPlaneHeight) + " WidthPx: " + std::to_string(filmPlaneWidth) + "\n";
	return result;
}

Camera::~Camera()
{
}
