#pragma once
#include "Point.h"
#include "Ray.h"
#include "World.h"

using Eigen::Matrix4f;

class Camera
{
public:
	Point position;
	Vector3f lookat;
	Vector3f up;
	Matrix4f viewTransform;
	float focalLength;
	int imageHeightPx;
	int imageWidthPx;
	float filmPlaneHeight;
	float filmPlaneWidth;


	Camera(Point p, Vector3f lookat, Vector3f up);

	void render(World world);

	~Camera();
};

