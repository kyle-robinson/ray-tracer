#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#ifdef _WIN32
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

#define MEMORY_POOLS
#define THREAD_COUNT 8

#include <iostream>
#include "RayTracer.h"
#include "../Utility/Sphere.h"
#include "../FileIO/JsonLoader.h"
#include "../Memory/MemoryPool.h"

class Renderer
{
public:
	void SetResolution( float width, float height );

	void Render_Basic();
	void Render_Shrinking();
	void Render_SmoothScaling();
	void Render_JsonFile( const char* filepath );

#ifdef MEMORY_POOLS
	void CreatePools( Heap* pChunkHeap, Heap* pCharHeap );
	void DeletePools();
#endif

private:
	void Render( const Sphere* spheres, unsigned iteration, unsigned sphereCount = 4u );

#ifdef MEMORY_POOLS
	MemoryPool* m_pChunkPool;
	MemoryPool* m_pCharPool;
#endif
	RayTracer m_rayTracer;

	float m_fWidth;
	float m_fHeight;
};

#endif