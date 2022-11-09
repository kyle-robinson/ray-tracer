#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#define THREAD_COUNT 8

#if defined __linux__ || defined __APPLE__
// "Compiled for Linux
#else
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

#include <iostream>
#include "Sphere.h"
#include "RayTracer.h"

class Renderer
{
public:
	void Render_Basic();
	void Render_Shrinking();
	void Render_SmoothScaling();
private:
	void Render( const std::vector<Sphere> &spheres, uint_fast32_t iteration );
	RayTracer m_rayTracer;
};

#endif