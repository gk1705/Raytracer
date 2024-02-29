#include "Color.h"

#include <algorithm>

Color Color::operator*(float scalar) const
{
	return { static_cast<Uint8>(r * scalar), static_cast<Uint8>(g * scalar), static_cast<Uint8>(b * scalar), a };
}

void Color::clampMultiplyFloat(float term)
{
	r = static_cast<Uint8>(std::min(r * term, 255.0f));
	g = static_cast<Uint8>(std::min(g * term, 255.0f));
	b = static_cast<Uint8>(std::min(b * term, 255.0f));
}

void Color::clampMultiplyColor(const Color& other)
{
	r = static_cast<Uint8>(std::min(r * other.r, 255));
	g = static_cast<Uint8>(std::min(g * other.g, 255));
	b = static_cast<Uint8>(std::min(b * other.b, 255));
}

void Color::clampAdd(const Color& other)
{
	r = static_cast<Uint8>(std::min(r + other.r, 255));
	g = static_cast<Uint8>(std::min(g + other.g, 255));
	b = static_cast<Uint8>(std::min(b + other.b, 255));
}

Color Color::operator*(const Color& other) const
{
	const Uint8 newR = static_cast<Uint8>(std::min((r * other.r) / 255, 255));
	const Uint8 newG = static_cast<Uint8>(std::min((g * other.g) / 255, 255));
	const Uint8 newB = static_cast<Uint8>(std::min((b * other.b) / 255, 255));
	return { newR, newG, newB, a };
}

Color Color::operator+(const Color& other) const
{
	const Uint8 newR = static_cast<Uint8>(std::min(r + other.r, 255));
	const Uint8 newG = static_cast<Uint8>(std::min(g + other.g, 255));
	const Uint8 newB = static_cast<Uint8>(std::min(b + other.b, 255));
	return { newR, newG, newB, a };
}