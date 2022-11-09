#include "Renderer.h"
#include "ThreadManager.h"

// Windows only
#include <sstream>
#include <string.h>

#pragma region RENDER_SETUP
void Renderer::Render_Basic()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
	spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // The radius paramter is the value we will change
	spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
	spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );

	// This creates a file, titled 1.ppm in the current working directory
	Render( spheres, 1U );
	std::cout << "Rendered and saved spheres0.ppm" << '\n' << '\n';
}

void Renderer::Render_Shrinking()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for ( uint_fast32_t i = 0U; i < 4U; ++i )
	{
		switch ( i )
		{
		case 0:
			spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // The radius paramter is the value we will change
			spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 1:
			spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 3.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius--
			spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 2:
			spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 2.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius--
			spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		case 3:
			spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 1.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius--
			spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
			spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );
			break;
		}

		Render( spheres, i );
		std::cout << "Rendered and saved spheres" << std::to_string( i ) << ".ppm" << '\n';
		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}

	std::cout << '\n';
}

void Renderer::Render_SmoothScaling()
{
	std::vector<Sphere> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for ( uint_fast32_t i = 0U; i <= 100U; ++i )
	{
		spheres.emplace_back( Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f,         Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f ) );
		spheres.emplace_back( Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), (float)i / 50.0f, Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ) ); // Radius++ change here
		spheres.emplace_back( Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,             Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f ) );
		spheres.emplace_back( Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,             Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f ) );

		Render( spheres, i );
		std::cout << "Rendered and saved spheres" << std::to_string( i ) << ".ppm" << '\n';
		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}

	std::cout << '\n';
}
#pragma endregion

#pragma region RENDER_MAIN
//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]
void Renderer::Render( const std::vector<Sphere> &spheres, uint_fast32_t iteration )
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
						*pixel = m_rayTracer.Trace( Vec3f( 0.0f ), raydir, spheres, 0U );
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
#pragma endregion