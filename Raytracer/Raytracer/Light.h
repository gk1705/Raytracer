#pragma once
#include "Color.h"
#include "Vector3.h"

class PointLight
{
public:
	Vector3 position;
	Color color;
	float intensity;
};

class DirectionalLight
{
public:
	Vector3 direction;
	Color color;
	float intensity;
};

class AmbientLight
{
public:
	Color color;
	float intensity;
};