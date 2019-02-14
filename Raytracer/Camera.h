#pragma once
#include "Point.h"
#include "Ray.h"
#include "World.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Camera
{
private:
	float focalLength;
	int imageHeightPx;
	int imageWidthPx;
	float filmPlaneHeight;
	float filmPlaneWidth;

public:
	Point position;
	RowVector3f lookat;
	RowVector3f up;
	Matrix<float, 4, 4, RowMajor> viewTransform;

	Camera(Point p, RowVector3f lookat, RowVector3f up);

	void render(World world);
	void setImageDim(int w, int h);
	void setFilmPlaneDim(int fov, float aspect);
	void setFocalLength(float f);

	std::string toString();

	~Camera();
};

