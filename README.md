# Raytracer
Ray tracing is a rendering technique for generating images by tracing the path of light through pixels in an image plane.

## Core Components:
* **Raytracer Implementation:** 
	- The core ray tracing logic is contained within Raytracer.cpp and Raytracer.h, which encompasses a set of functions dedicated to ray tracing operations.
* **Camera System:**
	- Camera.cpp and Camera.h, along with CameraController.cpp and CameraController.h, are a first person camera system for navigating or viewing the scene.
* **Mathematical Utilities:**
	- Vector3.cpp and Vector3.h for 3D vector operations, Quaternion.cpp and Quaternion.h for quaternion (used for camera rotation without gimbal lock)
* **Concurrency:**
	- ThreadPool.cpp and ThreadPool.h to speed up the rendering process by utilizing multiple CPU cores.
* **Light and Color:**
	- Light.h for defining light sources, Color.cpp, and Color.h for color operations.
* **Geometric Objects:**
	- Sphere.h, one geometric primitive (spheres) can be rendered by the ray tracer for now.

## Dependencies and External Libraries:
* **SDL 2:**
	- x64 SDL headers and libraries used for creating windows, handling events, and rendering the image
* **Visual Studio:**
	- project is set up to be developed with Microsoft Visual Studio