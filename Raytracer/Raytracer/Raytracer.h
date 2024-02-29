#pragma once

#include <cmath>
#include <limits>
#include <SDL.h>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "ThreadPool.h"
#include "Vector3.h"

// TODO: fix the additive light color
inline bool epsilonEquals(float a, float b)
{
	return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

class Raytracer
{
public:
	Raytracer(Camera& camera, float minDistance, float maxDistance, float aspectRatio, float fov);
	void render(SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture);
private:
	std::vector<Sphere> spheres;
	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> directionalLights;
	std::vector<AmbientLight> ambientLights;

	Camera& camera;
	float minDistance;
	float maxDistance;
	float aspectRatio;
	float fov;
	float halfFovTan;

	const int recursionLimit = 3;
	ThreadPool threadPool;

	void renderProjection(SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture);
	float intersectRaySphere(const Vector3& origin, const Vector3& direction, const Sphere& sphere) const;
	float computeBlinPhong(const Vector3& lightDir, const Vector3& normal, const Vector3& view, float lightIntensity, float lambertTerm, float specularTerm) const;
	static Vector3 reflectRay(const Vector3& direction, const Vector3& normal);
	bool isShadowed(const Vector3& point, const Vector3& lightDir, float distanceToLight) const;
	float computeLightingIntensity(const Vector3& point, const Vector3& normal, const Vector3& view, const Sphere& sphere);
	void findClosestIntersection(const Vector3& origin, const Vector3& direction, float& closest, Sphere& closestSphere) const;
	Color calculateLightingColor(const Vector3& point, const Vector3& normal, const Vector3& view, const Sphere& sphere);
	Color traceRay(const Vector3& origin, const Vector3& direction, int recursionDepth);

	static Uint32* getPixel(const SDL_Surface* surface, int x, int y);
	static void setPixel(const SDL_Surface* surface, int x, int y, Color color);
};