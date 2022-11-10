#include "JsonData.h"

JsonData::JsonData( int spherecount, int framecount )
{
	frameCount = framecount;
	sphereCount = spherecount;
	sphereArr = new Sphere[spherecount];
	sphereEndPositions = new Vec3f[spherecount];
	sphereMovementsPerFrame = new Vec3f[spherecount];
	sphereEndColor = new Vec3f[spherecount];
	sphereColorPerFrame = new Vec3f[spherecount];
}

void JsonData::Cleanup()
{
	delete sphereArr;
	sphereArr = nullptr;
	
	delete sphereEndPositions;
	sphereEndPositions = nullptr;
	
	delete sphereMovementsPerFrame;
	sphereMovementsPerFrame = nullptr;
	
	delete sphereEndColor;
	sphereEndColor = nullptr;

	delete sphereColorPerFrame;
	sphereColorPerFrame = nullptr;
}

void JsonData::CalculateSphereMovements()
{
	// Iterate over each sphere
	for ( unsigned i = 0u; i < sphereCount; i++ )
	{
		// Calculate amount of movement each frame based on lerp
		Vec3f diff = sphereEndPositions[i] - sphereArr[i].center;
		diff.x /= frameCount;
		diff.y /= frameCount;
		diff.z /= frameCount;
		sphereMovementsPerFrame[i] = diff;
	}
}

void JsonData::CalculateSphereColor()
{
	// Iterate over each sphere
	for ( unsigned i = 0u; i < sphereCount; i++ )
	{
		// Lerp between start and end colors
		Vec3f diff = sphereEndColor[i] - sphereArr[i].surfaceColor;
		diff.x /= frameCount;
		diff.y /= frameCount;
		diff.z /= frameCount;
		sphereColorPerFrame[i] = diff;
	}
}