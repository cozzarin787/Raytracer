#include "Checkerboard.h"

using Eigen::RowVector2f;

Checkerboard::Checkerboard(Color c1, Color c2, Point u, Point v, Point w, Color specColor, float ambient, float diffuse, float specular, float specHighlight)
{
	this->color1 = c1;
	this->color2 = c2;

	this->triU = u;
	this->triV = v;
	this->triW = w;

	this->k_a = ambient;	// ambient
	this->k_d = diffuse;	// diffuse
	this->k_s = specular;	// specular
	this->k_e = specHighlight;	// spec highlight
	this->C_s = specColor;  // specular color
}

Color Checkerboard::illuminate(IntersectData interData)
{
	// Checkerboard checker size constant
	float checkerWidth = 1.0f / 32.0f;
	float checkerHeight = 1.0f / 16.0f;

	// Convert intersection point into barycentric coordinates
	RowVector3f v0 = p1.vector() - p0.vector(), v1 = p2.vector() - p0.vector(), v2 = interData.P.vector() - p0.vector();
	float d00 = v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);
	float denom = d00 * d11 - d01 * d01;
	float u = (d11 * d20 - d01 * d21) / denom;
	float v = (d00 * d21 - d01 * d20) / denom;
	float w = 1.0f - u - v;
	RowVector3f baryInterPoint = RowVector3f(u, v, w);

	// Calculate u-v mapping of point onto checkerboard texture
	RowVector3f
		uT = baryInterPoint[0] * this->triU.vector(),
		vT = baryInterPoint[1] * this->triV.vector(),
		wT = baryInterPoint[2] * this->triW.vector(),
		T = uT + vT + wT;

	RowVector2f uv = RowVector2f(T[0],T[2]);

	// Determine which color of the checkerboard was intersected
	Color C_o;

	bool rowEven, colEven;
	int row = (int)(uv[0] / checkerWidth), col = (int)(uv[1] / checkerHeight);
	if (row % 2 == 0)
	{
		rowEven = true;
	}
	else
	{
		rowEven = false;
	}

	if (col % 2 == 0)
	{
		colEven = true;
	}
	else
	{
		colEven = false;
	}

	if (rowEven)
	{
		if (colEven)
		{
			C_o = this->color2;
		}
		else
		{
			C_o = this->color1;
		}
	}
	else
	{
		if (colEven)
		{
			C_o = this->color1;
		}
		else
		{
			C_o = this->color2;
		}
	}


	std::vector<RowVector3f> L;
	for (int i = 0; i < interData.lights.size(); i++)
	{
		L.push_back(interData.lights[i].color.vector());
	}

	// Ambient
	RowVector3f ambient = this->k_a * C_o.vector().array() * interData.ambientLight.vector().array();

	RowVector3f diffuse = RowVector3f(0, 0, 0);
	RowVector3f specular = RowVector3f(0, 0, 0);

	// If you can see light from point of intersection:
	if (!interData.lights.empty())
	{
		// Enumerate through all light direct light sources
		for (int i = 0; i < interData.lights.size(); i++)
		{
			// Diffuse
			RowVector3f L_iC_o = L[i].array() * C_o.vector().array();
			diffuse += (L_iC_o * (interData.S[i].dot(interData.N)));

			// Specular
			RowVector3f L_iC_s = L[i].array() * this->C_s.vector().array();
			specular += (L_iC_s * pow(interData.R[i].direction.dot(interData.V), this->k_e));
		}
		diffuse = this->k_d * diffuse;
		specular = this->k_s * specular;
	}
	RowVector3f totalRad = ambient + diffuse + specular;
	return Color(totalRad[0], totalRad[1], totalRad[2]);
}

std::string Checkerboard::toString()
{
	std::string s1 = std::to_string(this->k_a);
	std::string s2 = std::to_string(this->k_d);
	std::string s3 = std::to_string(this->k_s);
	std::string s4 = std::to_string(this->k_e);
	std::string s5 = this->color1.toString();
	std::string s6 = this->C_s.toString();
	std::string s7 = this->color2.toString();

	std::string s8 = this->triU.toString();
	std::string s9 = this->triV.toString();
	std::string s10 = this->triW.toString();
	std::string s11 = this->p0.toString();
	std::string s12 = this->p1.toString();
	std::string s13 = this->p2.toString();

	return std::string("Checkerboard:\n Ambient: " +
		s1 + "\n Diffuse: " +
		s2 + "\n Specular: " +
		s3 + "\n Spec Highlight: " +
		s4 + "\n Object Color 1: " +
		s5 + "\n Object Color 2: " +
		s7 + "\n Spec Color: " +
		s6 + "\n Triangle Coord: " +
		s11 + "\n Triangle Coord: " +
		s12 + "\n Triangle Coord: " +
		s13 + "\n Texture Coord U: " +
		s8 + "\n Texture Coord V: " +
		s9 + "\n Texture Coord W: " +
		s10 + "\n");
}

Checkerboard::~Checkerboard()
{
}
