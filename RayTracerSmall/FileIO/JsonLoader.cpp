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
	if ( HasAttribute( &jsonFile, "Frame Count" ) )
		noFrames = jsonFile["Frame Count"];
	else
	{
		std::cout << "[ERROR] JsonLoader::LoadSphereInfo\n";
		std::cout << "Json file does not specify the number of frames!\n";

	}

	if ( HasAttribute( &jsonFile, "Sphere Count" ) )
		noSpheres = jsonFile["Sphere Count"];
	else
	{
		std::cout << "[ERROR] JsonLoader::LoadSphereInfo\n"; 
		std::cout << "Json file does not specify number of spheres!\n";
	}

	// Object to get json data
	JsonData* animInfo = new JsonData( noSpheres, noFrames );
	nlohmann::json sphereArr = jsonFile["Spheres"];

	for ( unsigned i = 0u; i < noSpheres; i++ )
	{
		// Get json object for sphere
		nlohmann::json sphere = sphereArr.at( i );

		// Setup sphere attributes
		if ( HasAttribute( &sphere, "Start Position" ) )
			animInfo->sphereArr[i].center = LoadVec3f( sphere["Start Position"] );

		if ( HasAttribute( &sphere, "End Position" ) )
			animInfo->sphereEndPositions[i] = LoadVec3f( sphere["End Position"] );

		if ( HasAttribute( &sphere, "Start Color" ) )
			animInfo->sphereArr[i].surfaceColor = LoadVec3f( sphere["Start Color"] );

		if ( HasAttribute( &sphere, "End Color" ) )
			animInfo->sphereEndColor[i] = LoadVec3f( sphere["End Color"] );

		if ( HasAttribute( &sphere, "Radius" ) )
		{
			float radius = sphere["Radius"];
			animInfo->sphereArr[i].radius = radius;
			animInfo->sphereArr[i].radius2 = radius * radius;
		}

		if ( HasAttribute( &sphere, "Reflection" ) )
			animInfo->sphereArr[i].reflection = sphere["Reflection"];

		if ( HasAttribute( &sphere, "Transparency" ) )
			animInfo->sphereArr[i].transparency = sphere["Transparency"];

		if ( HasAttribute( &sphere, "Emission Color" ) )
			animInfo->sphereArr[i].emissionColor = LoadVec3f( sphere["Emission Color"] );
	}

	//Calculate the sphere movement and color values accross each frame
	animInfo->CalculateSpherePositions();
	animInfo->CalculateSphereColors();

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