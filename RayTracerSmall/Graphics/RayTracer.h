#pragma once
#ifndef RAYTRACER_H
#define RAYTRACER_H

//[comment]
// This variable controls the maximum recursion depth
//[/comment]
#define MAX_RAY_DEPTH 5

#include <vector>
#include "../Utility/Vec3.h"
#include "../Utility/Sphere.h"

class RayTracer
{
public:
	Vec3f Trace(
	const Vec3f& rayorig,
	const Vec3f& raydir,
	const Sphere* spheres,
	const unsigned& depth,
	const unsigned& size );
private:
	float Mix( const float &a, const float &b, const float &mix );
};

#endif