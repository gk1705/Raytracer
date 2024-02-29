#pragma once
#include "Color.h"
#include "Vector3.h"

class Sphere
{
public:
	Vector3 center;
	float radius;
	Color color;
	float lambert;
	float specular;
	float reflectivity;
};