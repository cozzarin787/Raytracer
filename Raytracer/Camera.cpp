#include "Camera.h"

Camera::Camera(Point p, Vector3f lookat, Vector3f up)
{
	this->position = p;
	this->lookat = lookat;
	this->up = up;

	// construct view transform
	Vector3f n = p.vector() - lookat.normalized();
	Vector3f u = (up.cross(n)).normalized();
	Vector3f v = n.cross(u);

	this->viewTransform.row(0) << u[0], u[1], u[2], -(p.vector().dot(u));
	this->viewTransform.row(1) << v[0], v[1], v[2], -(p.vector().dot(v));
	this->viewTransform.row(2) << n[0], n[1], n[2], -(p.vector().dot(n));
	this->viewTransform.row(3) << 0, 0, 0, 1;
}

void Camera::render(World world)
{
	// TODO
}

Camera::~Camera()
{
}
