#pragma once
#include <SDL_stdinc.h>

class Color
{
public:
	Uint8 r, g, b, a;

	Color operator*(float scalar) const;
	void clampMultiplyFloat(float term);
	void clampMultiplyColor(const Color& other);
	void clampAdd(const Color& other);
	Color operator*(const Color& other) const;
	Color operator+(const Color& other) const;
};
