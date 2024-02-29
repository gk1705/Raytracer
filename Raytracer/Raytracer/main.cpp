#include <iostream>

#include <SDL.h>
#include <chrono>

#include "CameraController.h"
#include "Raytracer.h"

#undef main

int init(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& surface, SDL_Texture*& texture, int windowWidth, int windowHeight)
{
	window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	surface = SDL_CreateRGBSurface(0, windowWidth, windowHeight, 32, 0, 0, 0, 0);
	if (!surface)
	{
		std::cerr << "SDL_CreateRGBSurface Error: " << SDL_GetError() << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	return 0;
}

void computeKeyboardInput(const CameraController& cameraController, float speed, float deltaTimeSec)
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_W])
	{
		cameraController.moveForward(speed * deltaTimeSec);
	}
	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S])
	{
		cameraController.moveForward(-speed * deltaTimeSec);
	}
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
	{
		cameraController.moveRight(-speed * deltaTimeSec);
	}
	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D])
	{
		cameraController.moveRight(speed * deltaTimeSec);
	}
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE])
	{
		cameraController.moveUp(speed * deltaTimeSec);
	}
	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL])
	{
		cameraController.moveUp(-speed * deltaTimeSec);
	}
}

void computeMouseMovement(Camera& camera, const SDL_Event& event, float sensitivity, float deltaTimeSec)
{
	const float deltaYaw = -event.motion.xrel * sensitivity * deltaTimeSec;
	const float deltaPitch = -event.motion.yrel * sensitivity * deltaTimeSec;
	camera.rotate(deltaYaw, deltaPitch);
}

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	constexpr int windowWidth = 800;
	constexpr int windowHeight = 600;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	if (init(window, renderer, surface, texture, windowWidth, windowHeight) != 0)
	{
		std::cerr << "Failed to initialize SDL" << std::endl;
		return 1;
	}

	Camera camera;
	const CameraController cameraController(camera, 0.02f);

	constexpr float sensitivity = 2.5f;
	constexpr float speed = 3.f;

	constexpr float minDistance = 0.01f;
	constexpr float maxDistance = std::numeric_limits<float>::max();
	const float aspectRatio = static_cast<float>(surface->w) / surface->h;
	constexpr float fov = 45.f; // field of View in degrees
	Raytracer raytracer(camera, minDistance, maxDistance, aspectRatio, fov);

	constexpr int FPS = 60; // default FPS
	constexpr int frameDelay = 1000 / FPS; // delay in ms per frame to achieve the target FPS

	SDL_Event event;
	bool running = true;
	Uint32 frameStart;
	int frameTime = 0;

	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	float deltaTimeSec = 0;
	while (running)
	{
		frameStart = SDL_GetTicks64();

		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			case SDL_MOUSEMOTION:
			{
				computeMouseMovement(camera, event, sensitivity, deltaTimeSec);
				break;
			}
			default:
				break;
			}
		}

		computeKeyboardInput(cameraController, speed, deltaTimeSec);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		raytracer.render(renderer, surface, texture);
		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks64() - frameStart;
		//std::cout << "Frame time: " << frameTime << "ms" << std::endl;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
			deltaTimeSec = static_cast<float>(frameTime + (frameDelay - frameTime)) / 1000.f;
		}
		else
		{
			deltaTimeSec = static_cast<float>(frameTime) / 1000.f;
		}
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}