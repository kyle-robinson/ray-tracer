#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#define MEMORY_POOLS
#define THREAD_COUNT 8
#define WIDTH 640.0f
#define HEIGHT 480.0f

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
#include "MemoryPool.h"

class Renderer
{
public:
	void Render_Basic();
	void Render_Shrinking();
	void Render_SmoothScaling();

#ifdef MEMORY_POOLS
	void CreatePools( Heap* pChunkHeap, Heap* pCharHeap );
	void DeletePools();
#endif
private:
	void Render( const std::vector<Sphere> &spheres, uint_fast32_t iteration );

#ifdef MEMORY_POOLS
	MemoryPool* m_pChunkPool;
	MemoryPool* m_pCharPool;
#endif
	RayTracer m_rayTracer;
};

#endif