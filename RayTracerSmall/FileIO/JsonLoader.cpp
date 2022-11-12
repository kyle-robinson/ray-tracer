#include "JsonLoader.h"
#include "../Utility/Console.h"

JsonData* JsonLoader::LoadSphereInfo( const char* filepath )
{
	Console::SetColor( Console::Color::RED );
	std::fstream file( filepath );
	
	nlohmann::json jsonFile;
	file >> jsonFile;

	unsigned noFrames = 0u;
	unsigned noSpheres = 0u;

	// Setup main data attributes
	if ( HasAttribute( &jsonFile, "frameCount" ) )
		noFrames = jsonFile["frameCount"];
	else
	{
		std::cout << "[ERROR] JsonLoader::LoadSphereInfo\n";
		std::cout << "Json file does not specify the number of frames!\n";

	}

	if ( HasAttribute( &jsonFile, "sphereCount" ) )
		noSpheres = jsonFile["sphereCount"];
	else
	{
		std::cout << "[ERROR] JsonLoader::LoadSphereInfo\n"; 
		std::cout << "Json file does not specify number of spheres!\n";
	}

	// Object to get json data
	JsonData* animInfo = new JsonData( noSpheres, noFrames );
	nlohmann::json sphereArr = jsonFile["spheres"];

	for ( unsigned i = 0u; i < noSpheres; i++ )
	{
		// Get json object for sphere
		nlohmann::json sphere = sphereArr.at( i );

		// Setup sphere attributes
		if ( HasAttribute( &sphere, "startPos" ) )
			animInfo->sphereArr[i].center = LoadVec3f( sphere["startPos"] );

		if ( HasAttribute( &sphere, "endPos" ) )
			animInfo->sphereEndPositions[i] = LoadVec3f( sphere["endPos"] );

		if ( HasAttribute( &sphere, "surfaceColor" ) )
			animInfo->sphereArr[i].surfaceColor = LoadVec3f( sphere["surfaceColor"] );

		if ( HasAttribute( &sphere, "endSurfaceColor" ) )
			animInfo->sphereEndColor[i] = LoadVec3f( sphere["endSurfaceColor"] );

		if ( HasAttribute( &sphere, "radius" ) )
		{
			float radius = sphere["radius"];
			animInfo->sphereArr[i].radius = radius;
			animInfo->sphereArr[i].radius2 = radius * radius;
		}

		if ( HasAttribute( &sphere, "reflection" ) )
			animInfo->sphereArr[i].reflection = sphere["reflection"];

		if ( HasAttribute( &sphere, "transparency" ) )
			animInfo->sphereArr[i].transparency = sphere["transparency"];

		if ( HasAttribute( &sphere, "emmisionColor" ) )
			animInfo->sphereArr[i].emissionColor = LoadVec3f( sphere["emmisionColor"] );
	}

	//Calculate the sphere movement and color values accross each frame
	animInfo->CalculateSphereMovements();
	animInfo->CalculateSphereColor();

	Console::SetColor( Console::Color::WHITE );
	return animInfo;
}

bool JsonLoader::HasAttribute( nlohmann::json* file, const std::string& key )
{
	if ( file->contains( key ) ) return true;
	return false;
}

Vec3f JsonLoader::LoadVec3f( const std::vector<float>& vec )
{
	return Vec3f( vec[0], vec[1], vec[2] );
}