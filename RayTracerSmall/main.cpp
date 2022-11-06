// [header]
// A very basic raytracer example.
// [/header]
// [compile]
// c++ -o raytracer -O3 -Wall raytracer.cpp
// [/compile]
// [ignore]
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// [/ignore]
#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
// Windows only
#include <algorithm>
#include <sstream>
#include <string.h>

#include "Timer.h"
#include "Sphere.h"
#include "Console.h"
#include "HeapManager.h"
#include "MemoryManager.h"
#include "ThreadManager.h"

#define THREAD_COUNT 8

#if defined __linux__ || defined __APPLE__
// "Compiled for Linux
#else
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

//[comment]
// This variable controls the maximum recursion depth
//[/comment]
#define MAX_RAY_DEPTH 5

float mix( const float &a, const float &b, const float &mix )
{
	return b * mix + a * ( 1.0f - mix );
}

//[comment]
// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene.
// If the ray intersects an object, we compute the intersection point, the normal
// at the intersection point, and shade this point using this information.
// Shading depends on the surface property (is it transparent, reflective, diffuse).
// The function returns a color for the ray. If the ray intersects an object that
// is the color of the object at the intersection point, otherwise it returns
// the background color.
//[/comment]
Vec3f trace(
	const Vec3f &rayorig,
	const Vec3f &raydir,
	const std::vector<Sphere> &spheres,
	const uint_fast32_t &depth )
{
	//if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
	float tnear = INFINITY;
	const Sphere* sphere = NULL;

	// find intersection of this ray with the sphere in the scene
	for ( uint_fast32_t i = 0U; i < (uint_fast32_t)spheres.size(); ++i )
	{
		float t0 = INFINITY, t1 = INFINITY;
		if ( spheres[i].intersect( rayorig, raydir, t0, t1 ) )
		{
			if ( t0 < 0 ) t0 = t1;
			if ( t0 < tnear )
			{
				tnear = t0;
				sphere = &spheres[i];
			}
		}
	}

	// if there's no intersection return black or background color
	if ( !sphere ) return Vec3f( 2.0f );
	Vec3f surfaceColor = 0.0f; // color of the ray/surfaceof the object intersected by the ray
	Vec3f phit = rayorig + raydir * tnear; // point of intersection
	Vec3f nhit = phit - sphere->center; // normal at the intersection point
	
	// normalize normal direction
	// If the normal and the view direction are not opposite to each other
	// reverse the normal direction. That also means we are inside the sphere so set
	// the inside bool to true. Finally reverse the sign of IdotN which we want
	// positive.
	nhit.normalize();

	float bias = 1e-4; // add some bias to the point from which we will be tracing
	bool inside = false;
	if ( raydir.dot( nhit ) > 0.0f ) nhit = -nhit, inside = true;
	if ( ( sphere->transparency > 0.0f || sphere->reflection > 0.0f ) && depth < MAX_RAY_DEPTH )
	{
		float facingratio = -raydir.dot( nhit );
		// change the mix value to tweak the effect
		float fresneleffect = mix( powf( 1.0f - facingratio, 3.0f ), 1.0f, 0.1f );
		// compute reflection direction (not need to normalize because all vectors are already normalized)
		Vec3f refldir = raydir - nhit * 2.0f * raydir.dot( nhit );
		refldir.normalize();
		Vec3f reflection = trace( phit + nhit * bias, refldir, spheres, depth + 1U );
		Vec3f refraction = 0.0f;

		// if the sphere is also transparent compute refraction ray (transmission)
		if ( sphere->transparency )
		{
			float ior = 1.1f, eta = ( inside ) ? ior : 1.0f / ior; // are we inside or outside the surface?
			float cosi = -nhit.dot( raydir );
			float k = 1.0f - eta * eta * ( 1.0f - cosi * cosi );
			Vec3f refrdir = raydir * eta + nhit * ( eta *  cosi - sqrt( k ) );
			refrdir.normalize();
			refraction = trace( phit - nhit * bias, refrdir, spheres, depth + 1U );
		}
		// the result is a mix of reflection and refraction (if the sphere is transparent)
		surfaceColor = (
			reflection * fresneleffect +
			refraction * ( 1.0f - fresneleffect ) * sphere->transparency ) * sphere->surfaceColor;
	}
	else
	{
		// it's a diffuse object, no need to raytrace any further
		for ( uint_fast32_t i = 0U; i < (uint_fast32_t)spheres.size(); ++i )
		{
			if ( spheres[i].emissionColor.x > 0.0f )
			{
				// this is a light
				Vec3f transmission = 1.0f;
				Vec3f lightDirection = spheres[i].center - phit;
				lightDirection.normalize();
				for ( uint_fast32_t j = 0U; j < (uint_fast32_t)spheres.size(); ++j )
				{
					if ( i != j )
					{
						float t0, t1;
						if ( spheres[j].intersect( phit + nhit * bias, lightDirection, t0, t1 ) )
						{
							transmission = 0.0f;
							break;
						}
					}
				}
				surfaceColor += sphere->surfaceColor * transmission *
					std::max( 0.0f, nhit.dot( lightDirection ) ) * spheres[i].emissionColor;
			}
		}
	}

	return surfaceColor + sphere->emissionColor;
}

//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]
void render( const std::vector<Sphere> &spheres, uint_fast32_t iteration )
{
	// Recommended Testing Resolution
	float width = 640.0f, height = 480.0f;
	// Recommended Production Resolution
	//float width = 1920.0f, height = 1080.0f;

	Vec3f *image = new Vec3f[width * height], *pixel = image;
	float invWidth = 1.0f / float( width ), invHeight = 1.0f / float( height );
	float fov = 30.0f, aspectratio = float( width ) / float( height );
	float angle = tanf( M_PI * 0.5f * fov / 180.0f );

	Vec3f** chunkArrs = new Vec3f * [THREAD_COUNT];
	char** charArrs = new char* [THREAD_COUNT];
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
		chunkArrs[i] = new Vec3f[( ( width * height ) / THREAD_COUNT )];
		charArrs[i] = new char[( ( width * height ) / THREAD_COUNT ) * 3];
	}

	// Trace rays
	int startY = 0;
	int endY = height / THREAD_COUNT;
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
		Vec3f* currentChunk = chunkArrs[i];
		char* currentArr = charArrs[i];
		ThreadManager::CreateThread( [=, &pixel]
			{
				for ( float y = startY; y < endY; ++y )
				{
					for ( float x = 0.0f; x < width; ++x, ++pixel )
					{
						float xx = ( 2.0f * ( ( x + 0.5f ) * invWidth ) - 1.0f ) * angle * aspectratio;
						float yy = ( 1.0f - 2.0f * ( ( y + 0.5f ) * invHeight ) ) * angle;
						Vec3f raydir( xx, yy, -1.0f );
						raydir.normalize();
						*pixel = trace( Vec3f( 0.0f ), raydir, spheres, 0U );
					}
				}

				int charIndex = 0;
				for ( uint_fast32_t j = 0u; j < ( width * height ) / THREAD_COUNT; ++j )
				{
					currentArr[charIndex] = (unsigned char)( ( 1.0f < currentChunk[j].x ? 1.0f : currentChunk[j].x ) * 255 );
					currentArr[charIndex + 1] = (unsigned char)( ( 1.0f < currentChunk[j].y ? 1.0f : currentChunk[j].y ) * 255 );
					currentArr[charIndex + 2] = (unsigned char)( ( 1.0f < currentChunk[j].z ? 1.0f : currentChunk[j].z ) * 255 );
					charIndex += 3;
				}
			});
		startY += height / THREAD_COUNT;
		endY += height / THREAD_COUNT;
	}
	ThreadManager::WaitForAllThreads();

	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::stringstream ss;
	ss << "./Resources/spheres" << std::to_string( iteration ) << ".ppm";
	std::string tempString = ss.str();
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs( filename, std::ios::out | std::ios::binary );
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
		ofs.write( charArrs[i], ( ( width * height ) / THREAD_COUNT ) * 3 );
		delete[] chunkArrs[i];
		delete[] charArrs[i];
		charArrs[i] = nullptr;
		charArrs[i] = nullptr;
	}
	ofs.close();

	delete[] chunkArrs;
	delete[] charArrs;
	delete[] image;

	image = nullptr;
	charArrs = nullptr;
	chunkArrs = nullptr;
}

void BasicRender()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
	spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // The radius paramter is the value we will change
	spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
	spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
	
	// This creates a file, titled 1.ppm in the current working directory
	render( spheres, 1U );
	std::cout << '\n';
}

void SimpleShrinking()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for ( uint_fast32_t i = 0U; i < 4U; ++i )
	{
		switch ( i )
		{
		case 0:
			spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // The radius paramter is the value we will change
			spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 1:
			spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 3.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius--
			spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 2:
			spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 2.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius--
			spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 3:
			spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f) );
			spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 1.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f) ); // Radius--
			spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f) );
			spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f) );
			break;
		}

		render( spheres, i );
		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}

	std::cout << '\n';
}

void SmoothScaling()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for ( uint_fast32_t i = 0U; i <= 100U; ++i )
	{
		spheres.push_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f), 10000.0f,         Vec3f( 0.20f, 0.20f, 0.20f), 0.0f, 0.0f ) );
		spheres.push_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f), (float)i / 50.0f, Vec3f( 1.00f, 0.32f, 0.36f), 1.0f, 0.5f ) ); // Radius++ change here
		spheres.push_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f), 2.0f,             Vec3f( 0.90f, 0.76f, 0.46f), 1.0f, 0.0f ) );
		spheres.push_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f), 3.0f,             Vec3f( 0.65f, 0.77f, 0.97f), 1.0f, 0.0f ) );
		render( spheres, i );
		std::cout << "Rendered and saved spheres" << std::to_string( i ) << ".ppm" << '\n';
		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}

	std::cout << '\n';
}

//[comment]
// In the main function, we will create the scene which is composed of 5 spheres
// and 1 light (which is also a sphere). Then, once the scene description is complete
// we render that scene, by calling the render() function.
//[/comment]
int main( int argc, char **argv )
{
	// This sample only allows one choice per program execution. Feel free to improve upon this
	srand( 13 );
	Timer timer;

	//BasicRender();
	//SimpleShrinking();
	SmoothScaling();

	// Print time taken to render
	Console::SetColor( Console::Color::CYAN );
	float timeElapsed = timer.Mark();
	std::cout << "[TIME ELAPSED]\t" << timeElapsed << " seconds\n";

	// Print heap information
	Console::SetColor( Console::Color::MAGENTA );
	std::cout << "\n\nHEAP INFORMATION\n\n";
	HeapManager::CheckAllHeaps();

	Console::SetColor( Console::Color::BLUE );
	std::cout << "\nDELETING HEAPS\n\n";
	HeapManager::DeleteHeaps();

	return 0;
}