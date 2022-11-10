#pragma once
#ifndef JSONDATA_H
#define JSONDATA_H

#include "Vec3.h"
#include "Sphere.h"

class JsonData
{
public:
	JsonData( int spherecount, int framecount );
	void Cleanup();
	void CalculateSphereMovements();
	void CalculateSphereColor();
	
	int frameCount;
	int sphereCount;

	// Array data for sphere animations
	Sphere* sphereArr;
	Vec3f* sphereEndPositions;
	Vec3f* sphereMovementsPerFrame;
	Vec3f* sphereEndColor;
	Vec3f* sphereColorPerFrame;
};

#endif