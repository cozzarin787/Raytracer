#pragma once
#include "Point.h"
#include "Ray.h"
#include "World.h"

using Eigen::Matrix4f;

class Camera
{
private:
	Matrix4f viewTransform;
	float focalLength;
	int imageHeightPx;
	int imageWidthPx;
	float filmPlaneHeight;
	float filmPlaneWidth;

public:
	Point position;
	Vector3f lookat;
	Vector3f up;


	Camera(Point p, Vector3f lookat, Vector3f up);

	void render(World world);
	void setImageDim(int h, int w);
	void setFilmPlaneDim(float h, float w);
	void setFocalLength(float f);

	std::string toString();

	~Camera();
};

