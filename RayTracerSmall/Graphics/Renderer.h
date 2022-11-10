#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#ifdef _WIN32
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

#define MEMORY_POOLS
#define THREAD_COUNT 8
#define WIDTH 640.0f
#define HEIGHT 480.0f

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
	void Render( const Sphere* spheres, unsigned iteration );

#ifdef MEMORY_POOLS
	MemoryPool* m_pChunkPool;
	MemoryPool* m_pCharPool;
#endif
	RayTracer m_rayTracer;
};

#endif