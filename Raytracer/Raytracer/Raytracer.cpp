#include "Raytracer.h"

#include <iostream>

Raytracer::Raytracer(Camera& camera, float minDistance, float maxDistance, float aspectRatio, float fov): camera(camera), minDistance(minDistance), maxDistance(maxDistance), aspectRatio(aspectRatio), fov(fov), threadPool(std::thread::hardware_concurrency())
{
	halfFovTan = tan(fov * 0.5 * M_PI / 180.0);

	constexpr Sphere sphere1 = { { 1.f, -1, 5 }, 0.5f, { 255, 0, 0, 0 }, 0.2f, 2, 0.2f };
	constexpr Sphere sphere2 = { { 1.5f, 0, 4 }, 0.5f, { 0, 255, 0, 0 }, 0.5f, 50, 0.4f };
	constexpr Sphere sphere3 = { { -1.5f, 0, 4 }, 0.5f, { 0, 0, 255, 0 }, 1.f, 500, 0.7f };
	spheres = { sphere1, sphere2, sphere3 };

	constexpr PointLight light = { { 2, 1, 0 }, { 255, 255, 255, 0 }, 0.5f };
	pointLights = { light };

	constexpr DirectionalLight dirLight = { { 1, 1, 1 }, { 255, 255, 255, 0 }, 0.3f };
	directionalLights = { dirLight };

	constexpr AmbientLight ambient = { { 255, 255, 255, 0 }, 0.3f };
	ambientLights = { ambient };
}

Uint32* Raytracer::getPixel(const SDL_Surface* surface, int x, int y)
{
	return reinterpret_cast<Uint32*>(static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * 4);
}

void Raytracer::setPixel(const SDL_Surface* surface, int x, int y, Color color)
{
	*getPixel(surface, x, y) = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
}

void Raytracer::render(SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture)
{
	SDL_LockSurface(surface);
	renderProjection(renderer, surface, texture);
	SDL_UnlockSurface(surface);
	SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}

void Raytracer::renderProjection(SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture)
{
	std::vector<std::future<void>> futures;
	for (int y = 0; y < surface->h; y++)
	{
		auto future = threadPool.enqueue([this, y, surface]
		{
			const Vector3 forward = camera.forward;
			const Vector3 right = camera.right;
			const Vector3 up = camera.up;

			for (int x = 0; x < surface->w; x++)
			{
				// calculate NDC coordinates and adjust for aspect ratio
				float ndcX = (x + 0.5f) / surface->w * 2.0f - 1.0f;
				float ndcY = 1.0f - (y + 0.5f) / surface->h * 2.0f; // flip y so +y is up
				ndcX *= aspectRatio * halfFovTan;
				ndcY *= halfFovTan;

				Vector3 rayDirection = forward + (right * ndcX) + (up * ndcY);
				rayDirection.normalize();
				const Color color = traceRay(camera.position, rayDirection, recursionLimit);
				setPixel(surface, x, y, color);
			}
		});

		futures.push_back(std::move(future));
	}

	for (auto& future : futures)
	{
		future.get();
	}
}

float Raytracer::intersectRaySphere(const Vector3& origin, const Vector3& direction, const Sphere& sphere) const
{
	const Vector3 co = origin - sphere.center;
	const float a = direction * direction;
	const float b = 2 * (co * direction);
	const float c = co * co - sphere.radius * sphere.radius;
	const float discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return -1;
	}
	else if (epsilonEquals(discriminant, 0))
	{
		return -b / (2 * a);
	}
	else
	{
		const float t1 = (-b - sqrt(discriminant)) / (2 * a);
		const float t2 = (-b + sqrt(discriminant)) / (2 * a);
		return t1 < t2 ? t1 : t2;
	}
}

float Raytracer::computeBlinPhong(const Vector3& lightDir, const Vector3& normal, const Vector3& view,
	float lightIntensity, float lambertTerm, float specularTerm) const
{
	float intensity = 0.0f;

	// diffuse lighting
	const float dot = normal * lightDir;
	if (dot > 0.0f)
	{
		intensity += lightIntensity * lambertTerm * dot;
	}

	// specular lighting
	if (specularTerm > 0.0f)
	{
		Vector3 halfVector = lightDir + view;
		halfVector.normalize();
		const float spec = std::pow(std::max(normal * halfVector, 0.f), specularTerm);
		intensity += lightIntensity * spec;
	}

	return intensity;
}

Vector3 Raytracer::reflectRay(const Vector3& direction, const Vector3& normal)
{
	return direction - normal * (2 * (direction * normal));
}

bool Raytracer::isShadowed(const Vector3& point, const Vector3& lightDir, float distanceToLight) const
{
	for (const auto& sphere : spheres)
	{
		const float distance = intersectRaySphere(point, lightDir, sphere);
		if (distance > minDistance && distance < distanceToLight)
		{
			return true;
		}
	}

	return false;
}

float Raytracer::computeLightingIntensity(const Vector3& point, const Vector3& normal, const Vector3& view,
	const Sphere& sphere)
{
	float intensity = 0.0f;

	for (const auto& ambient : ambientLights)
	{
		intensity += ambient.intensity;
	}

	for (auto& dirLight : directionalLights)
	{
		dirLight.direction.normalize();
		if (!isShadowed(point, dirLight.direction, maxDistance))
			intensity += computeBlinPhong(dirLight.direction, normal, view, dirLight.intensity, sphere.lambert, sphere.specular);
	}

	for (const auto& pointLight : pointLights)
	{
		Vector3 lightDir = pointLight.position - point;
		const float distance = lightDir.length();
		lightDir.x /= distance;
		lightDir.y /= distance;
		lightDir.z /= distance;
		if (!isShadowed(point, lightDir, distance))
			intensity += computeBlinPhong(lightDir, normal, view, pointLight.intensity, sphere.lambert, sphere.specular);
	}

	return intensity;
}

void Raytracer::findClosestIntersection(const Vector3& origin, const Vector3& direction, float& closest,
	Sphere& closestSphere) const
{
	for (const auto& sphere : spheres)
	{
		const float distance = intersectRaySphere(origin, direction, sphere);
		if (distance > minDistance && distance < closest)
		{
			closest = distance;
			closestSphere = sphere;
		}
	}
}

Color Raytracer::calculateLightingColor(const Vector3& point, const Vector3& normal, const Vector3& view,
	const Sphere& sphere)
{
	const float intensity = computeLightingIntensity(point, normal, view, sphere);
	Color white = { 255, 255, 255, 0 }; // TODO: colored light calculation
	white.clampMultiplyFloat(intensity);
	return sphere.color * white;
}

Color Raytracer::traceRay(const Vector3& origin, const Vector3& direction, int recursionDepth)
{
	Color color = { 0, 0, 0, 0 }; // background color
	float closest = maxDistance;
	Sphere closestSphere{};

	findClosestIntersection(origin, direction, closest, closestSphere);
	if (epsilonEquals(closest, maxDistance) || closest > maxDistance)
	{
		return color;
	}

	const Vector3 point = origin + direction * closest;
	Vector3 normal = point - closestSphere.center;
	normal.normalize();
	const Vector3 view = -camera.forward;
	color = calculateLightingColor(point, normal, view, closestSphere);

	const float reflectivity = closestSphere.reflectivity;
	if (recursionDepth <= 0 || epsilonEquals(reflectivity, 0.0f))
	{
		return color;
	}

	const Vector3 reflectedRay = reflectRay(direction, normal);
	const Color reflectedColor = traceRay(point, reflectedRay, recursionDepth - 1);

	return color * (1 - reflectivity) + reflectedColor * reflectivity;
}

