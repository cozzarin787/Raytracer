#include "Triangle.h"
#include "Checkerboard.h"

/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Modified by Jacob Cozzarin & Pablo Ordorica			*/
/* Function: bool inVoxel(Voxel v);						*/
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*   2019-03-07: modified for use in raytracer.			*/
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#include <math.h>
#include <stdio.h>

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

int planeBoxOverlap(float normal[3], float d, float maxbox[3])
{
	int q;
	float vmin[3], vmax[3];
	for (q = X; q <= Z; q++)
	{
		if (normal[q] > 0.0f)
		{
			vmin[q] = -maxbox[q];
			vmax[q] = maxbox[q];
		}
		else
		{
			vmin[q] = maxbox[q];
			vmax[q] = -maxbox[q];
		}
	}
	if (DOT(normal, vmin) + d > 0.0f) return 0;
	if (DOT(normal, vmax) + d >= 0.0f) return 1;

	return 0;
}

/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)             \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p2 = a*v2[Y] - b*v2[Z];                    \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)              \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p1 = a*v1[Y] - b*v1[Z];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return false;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)             \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p2 = -a*v2[X] + b*v2[Z];                       \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)              \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p1 = -a*v1[X] + b*v1[Z];                       \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return false;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)             \
    p1 = a*v1[X] - b*v1[Y];                    \
    p2 = a*v2[X] - b*v2[Y];                    \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)              \
    p0 = a*v0[X] - b*v0[Y];                \
    p1 = a*v1[X] - b*v1[Y];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return false;



Triangle::Triangle(Material* m, Point p0, Point p1, Point p2) : Object(m)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

Object::intersectResult Triangle::intersect(Ray r)
{
	float omega = 0.0f, u = 0.0f, v = 0.0f, w = 0.0f;
	Vector3f 
		e1 = p1.vector() - p0.vector(),
		e2 = p2.vector() - p0.vector(),
		T = r.origin.vector() - p0.vector(),
		P = r.direction.cross(e2),
		Q = T.cross(e1);

	float denom = P.dot(e1);
	if (denom == 0) 
	{
		return intersectResult(false);
	}

	Vector3f vec = Vector3f(
		Q.dot(e2),
		P.dot(T),
		Q.dot(r.direction));

	vec *= (1 / denom);
	omega = vec[0];

	if (omega <= epsilon)
	{
		return intersectResult(false);
	}

	u = vec[1];
	v = vec[2];
	w = 1 - u - v;
	if (w < 0)
	{
		// intersection behind camera
		return intersectResult(false);
	}
	else if (u < 0 || v < 0 || u + v > 1)
	{
		// intersection outside of the triangle
		return intersectResult(false);
	}
	else
	{
		// calc intersection point
		float xi = w * p0.x + u * p1.x + v * p2.x;
		float yi = w * p0.y + u * p1.y + v * p2.y;
		float zi = w * p0.z + u * p1.z + v * p2.z;
		Point i = Point(xi, yi, zi);

		// calc normal
		Vector3f normal = e1.cross(e2).normalized();

		return intersectResult(true, omega, this->mat, i, normal);
	}
}

void Triangle::transform(Matrix4f transMat)
{
	// Transform p0
	Vector4f p0Homo = this->p0.homogen();
	Vector4f p0PrimeHomo = transMat * p0Homo;
	float w = p0PrimeHomo[3];
	this->p0 = Point(p0PrimeHomo[0] / w, p0PrimeHomo[1] / w, p0PrimeHomo[2] / w);

	// Transform p1
	Vector4f p1Homo = this->p1.homogen();
	Vector4f p1PrimeHomo = transMat * p1Homo;
	w = p1PrimeHomo[3];
	this->p1 = Point(p1PrimeHomo[0] / w, p1PrimeHomo[1] / w, p1PrimeHomo[2] / w);

	// Transform p2
	Vector4f p2Homo = this->p2.homogen();
	Vector4f p2PrimeHomo = transMat * p2Homo;
	w = p2PrimeHomo[3];
	this->p2 = Point(p2PrimeHomo[0] / w, p2PrimeHomo[1] / w, p2PrimeHomo[2] / w);

	// Check to see if illumination model is a texture
	Checkerboard* check = dynamic_cast<Checkerboard*>(this->mat);
	if (check != NULL)
	{
		// Update object coordinates
		check->p0 = this->p0;
		check->p1 = this->p1;
		check->p2 = this->p2;
	}
}

bool Triangle::inVoxel(Voxel v)
{

	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	float v0[3], v1[3], v2[3];
	float min, max, d, p0, p1, p2, rad, fex, fey, fez;
	float normal[3], e0[3], e1[3], e2[3];
	float trivert1[3] = { this->p0.x, this->p0.y, this->p0.z };
	float trivert2[3] = { this->p1.x, this->p1.y, this->p1.z };
	float trivert3[3] = { this->p2.x, this->p2.y, this->p2.z };

	Vector3f spatialMedian = (v.max.vector() + v.min.vector()) / 2.0f;
	float boxcenter[3] = { spatialMedian[0], spatialMedian[1], spatialMedian[2] };
	float boxhalfsize[3] = { spatialMedian[0] - v.min.x, spatialMedian[1] - v.min.y, spatialMedian[2] - v.min.z };

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	SUB(v0, trivert1, boxcenter);
	SUB(v1, trivert2, boxcenter);
	SUB(v2, trivert3, boxcenter);

	/* compute triangle edges */
	SUB(e0, v1, v0);      /* tri edge 0 */
	SUB(e1, v2, v1);      /* tri edge 1 */
	SUB(e2, v0, v2);      /* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabs(e0[X]);
	fey = fabs(e0[Y]);
	fez = fabs(e0[Z]);
	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = fabs(e1[X]);
	fey = fabs(e1[Y]);
	fez = fabs(e1[Z]);
	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = fabs(e2[X]);
	fey = fabs(e2[Y]);
	fez = fabs(e2[Z]);
	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	FINDMINMAX(v0[X], v1[X], v2[X], min, max);
	if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return false;

	/* test in Y-direction */
	FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
	if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return false;

	/* test in Z-direction */
	FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
	if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	CROSS(normal, e0, e1);
	d = -DOT(normal, v0);  /* plane eq: normal.x+d=0 */
	if (!planeBoxOverlap(normal, d, boxhalfsize)) return false;

	return true;   /* box and triangle overlaps */
}

std::string Triangle::toString()
{
	std::string p0 = this->p0.toString();
	std::string p1 = this->p1.toString();
	std::string p2 = this->p2.toString();
	std::string m = this->mat->toString();
	return std::string("Triangle\n Material: " + m + "\n p0: " + p0 + "\n p1: " + p1 + "\n p2: " + p2);
}


Triangle::~Triangle()
{
}
