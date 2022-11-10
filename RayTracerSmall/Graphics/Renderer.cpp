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
	Vec3f *image = new Vec3f[WIDTH * HEIGHT], *pixel = image;
	float invWidth = 1.0f / WIDTH, invHeight = 1.0f / HEIGHT;
	float fov = 30.0f, aspectratio = WIDTH / HEIGHT;
	float angle = tanf( M_PI * 0.5f * fov / 180.0f );

	Vec3f** chunkArrs = new Vec3f * [THREAD_COUNT];
	char** charArrs = new char* [THREAD_COUNT];
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
#ifdef MEMORY_POOLS
		chunkArrs[i] = (Vec3f*)m_pChunkPool->Allocate( ( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * sizeof( Vec3f ) );
		charArrs[i] = (char*)m_pCharPool->Allocate( ( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * 3 );
#else
		chunkArrs[i] = new Vec3f[( ( WIDTH * HEIGHT ) / THREAD_COUNT )];
		charArrs[i] = new char[( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * 3];
#endif
	}

	// Trace rays
	int startY = 0;
	int endY = HEIGHT / THREAD_COUNT;
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
		Vec3f* currentChunk = chunkArrs[i];
		char* currentArr = charArrs[i];
		ThreadManager::CreateThread( [=, &pixel]
		{
#ifdef MEMORY_POOLS
			int index = 0;
#endif
			for ( float y = startY; y < endY; ++y )
			{
#ifdef MEMORY_POOLS
				for ( float x = 0.0f; x < WIDTH; ++x, index++ )
#else
				for ( float x = 0.0f; x < WIDTH; ++x, ++pixel )
#endif
				{
					float xx = ( 2.0f * ( ( x + 0.5f ) * invWidth ) - 1.0f ) * angle * aspectratio;
					float yy = ( 1.0f - 2.0f * ( ( y + 0.5f ) * invHeight ) ) * angle;
					Vec3f raydir( xx, yy, -1.0f );
					raydir.normalize();
#ifdef MEMORY_POOLS
					currentChunk[index] = m_rayTracer.Trace( Vec3f( 0.0f ), raydir, spheres, 0U );
#else
					*pixel = m_rayTracer.Trace( Vec3f( 0.0f ), raydir, spheres, 0U );
#endif
				}
			}

#ifdef MEMORY_POOLS
			int charIndex = 0;
			for ( uint_fast32_t i = 0u; i < ( WIDTH * HEIGHT ) / THREAD_COUNT; ++i )
			{
				currentArr[charIndex] = (unsigned char)( ( 1.0f < currentChunk[i].x ? 1.0f : currentChunk[i].x ) * 255 );
				currentArr[charIndex + 1] = (unsigned char)( ( 1.0f < currentChunk[i].y ? 1.0f : currentChunk[i].y ) * 255 );
				currentArr[charIndex + 2] = (unsigned char)( ( 1.0f < currentChunk[i].z ? 1.0f : currentChunk[i].z ) * 255 );
				charIndex += 3;
			}
#endif
		});
		startY += HEIGHT / THREAD_COUNT;
		endY += HEIGHT / THREAD_COUNT;
	}
	ThreadManager::WaitForAllThreads();

	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::stringstream ss;
	ss << "./Resources/spheres" << std::to_string( iteration ) << ".ppm";
	std::string tempString = ss.str();
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs( filename, std::ios::out | std::ios::binary );
	ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
	for ( uint_fast32_t i = 0U; i < THREAD_COUNT; i++ )
	{
		ofs.write( charArrs[i], ( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * 3 );
#ifdef MEMORY_POOLS
		m_pChunkPool->Free( chunkArrs[i] );
		m_pCharPool->Free( charArrs[i] );
#else
		delete[] chunkArrs[i];
		delete[] charArrs[i];
		charArrs[i] = nullptr;
		charArrs[i] = nullptr;
#endif
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

#ifdef MEMORY_POOLS
void Renderer::CreatePools( Heap* pChunkHeap, Heap* pCharHeap )
{
	m_pChunkPool = new( pChunkHeap ) MemoryPool( pChunkHeap, THREAD_COUNT, ( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * sizeof( Vec3f ) );
	m_pCharPool = new( pCharHeap ) MemoryPool( pCharHeap, THREAD_COUNT, ( ( WIDTH * HEIGHT ) / THREAD_COUNT ) * 3 );
}

void Renderer::DeletePools()
{
	delete m_pChunkPool;
	m_pChunkPool = nullptr;

	delete m_pCharPool;
	m_pCharPool = nullptr;
}
#endif