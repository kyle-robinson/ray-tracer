#include "Renderer.h"
#include "../Memory/ThreadManager.h"
#include <filesystem>

#ifdef _WIN32
#include <sstream>
#include <string.h>
#include <ppl.h>
#endif

#pragma region RENDER_SETUP
void Renderer::SetResolution( float width, float height )
{
	m_fWidth = width;
	m_fHeight = height;
}

void Renderer::Render_Basic()
{
	// Dynamic array is more efficient than vector
	Sphere* spheres = new Sphere[4];

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	spheres[0] = Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f );
	spheres[1] = Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ); // The radius paramter is the value we will change
	spheres[2] = Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f );
	spheres[3] = Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f );

	// This creates a file, titled 1.ppm in the current working directory
	Render( spheres, 1u );
	std::cout << "Rendered and saved spheres0.ppm" << '\n' << '\n';

	delete[] spheres;
	spheres = nullptr;
}

void Renderer::Render_Shrinking()
{
	// Dynamic array is more efficient than vector
	Sphere* spheres = new Sphere[4];

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	spheres[0] = Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f, Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f );
	spheres[1] = Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 4.0f,     Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ); // The radius paramter is the value we will change
	spheres[2] = Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,     Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f );
	spheres[3] = Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,     Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f );

	for ( unsigned i = 0u; i < 4u; ++i )
	{
		switch ( i )
		{
		case 0:
			spheres[1].radius = 4.0f;
			spheres[1].radius2 = 4.0f * 4.0f;
			break;
		case 1:
			spheres[1].radius = 3.0f;
			spheres[1].radius2 = 3.0f * 3.0f;
			break;
		case 2:
			spheres[1].radius = 2.0f;
			spheres[1].radius2 = 2.0f * 2.0f;
			break;
		case 3:
			spheres[1].radius = 1.0f;
			spheres[1].radius2 = 1.0f;
			break;
		}

		Render( spheres, i );
		std::cout << "Rendered and saved spheres" << std::to_string( i ) << ".ppm" << '\n';
	}

	delete[] spheres;
	spheres = nullptr;
	std::cout << '\n';
}

void Renderer::Render_SmoothScaling()
{
	// Dynamic array is more efficient than vector
	Sphere* spheres = new Sphere[4];
	
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	spheres[0] = Sphere( Vec3f( 0.0f, -10004.0f, -20.0f ), 10000.0f,     Vec3f( 0.20f, 0.20f, 0.20f ), 0.0f, 0.0f );
	spheres[1] = Sphere( Vec3f( 0.0f,  0.0f,     -20.0f ), 0.0f / 50.0f, Vec3f( 1.00f, 0.32f, 0.36f ), 1.0f, 0.5f ); // Radius++ change here
	spheres[2] = Sphere( Vec3f( 5.0f, -1.0f,     -15.0f ), 2.0f,         Vec3f( 0.90f, 0.76f, 0.46f ), 1.0f, 0.0f );
	spheres[3] = Sphere( Vec3f( 5.0f,  0.0f,     -25.0f ), 3.0f,         Vec3f( 0.65f, 0.77f, 0.97f ), 1.0f, 0.0f );

	for ( float r = 0.0f; r <= 100.0f; ++r )
	{
		float radius = r / 50.0f;
		spheres[1].radius = radius;
		spheres[1].radius2 = radius * radius;

		Render( spheres, r );
		std::cout << "Rendered and saved spheres" << std::to_string( (unsigned)r ) << ".ppm" << '\n';
	}

	delete[] spheres;
	spheres = nullptr;
	std::cout << '\n';
}

void Renderer::Render_JsonFile( const char* filepath )
{
	JsonData jsonData = *JsonLoader::LoadSphereInfo( filepath );
	for ( unsigned i = 0u; i < jsonData.frameCount; ++i )
	{
		for ( unsigned j = 0u; j < jsonData.sphereCount; ++j )
		{
			// Change sphere position and color
			jsonData.sphereArr[j].center += jsonData.sphereMovementsPerFrame[j];
			jsonData.sphereArr[j].surfaceColor += jsonData.sphereColorPerFrame[j];
		}

		Render( jsonData.sphereArr, i, jsonData.sphereCount );
		std::cout << "Rendered and saved spheres" << i << ".ppm" << '\n';
	}
	jsonData.Cleanup();
	std::cout << '\n';
}
#pragma endregion

#pragma region RENDER_MAIN
//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]
void Renderer::Render( const Sphere* spheres, unsigned iteration, unsigned sphereCount )
{
	float invWidth = 1.0f / m_fWidth;
	float invHeight = 1.0f / m_fHeight;

	float fov = 30.0f;
	float aspectratio = m_fWidth / m_fHeight;
	float angle = tanf( (float)M_PI * 0.5f * fov / 180.0f );

	Vec3f** chunkArrs = new Vec3f * [THREAD_COUNT];
	char** charArrs = new char* [THREAD_COUNT];

	for ( unsigned i = 0u; i < THREAD_COUNT; i++ )
	{
#ifdef MEMORY_POOLS
		chunkArrs[i] = (Vec3f*)m_pChunkPool->Allocate( unsigned( ( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * sizeof( Vec3f ) ) );
		charArrs[i] = (char*)m_pCharPool->Allocate( unsigned( ( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * 3u ) );
#else
		chunkArrs[i] = new Vec3f[unsigned( ( m_fWidth * m_fHeight ) / THREAD_COUNT )];
		charArrs[i] = new char[unsigned( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * 3u];
#endif
	}

	// Trace rays
	int startY = 0;
	int endY = (int)m_fHeight / THREAD_COUNT;
	for ( unsigned i = 0; i < THREAD_COUNT; ++i )
	{
		Vec3f* currentChunk = chunkArrs[i];
		char* currentArr = charArrs[i];
		ThreadManager::CreateThread( [&, currentChunk, currentArr, startY, endY]() -> void
		{
#if defined( _WIN32 ) && defined( PARALLEL_FOR )
			concurrency::parallel_for( startY, endY, [&, currentChunk, currentArr, startY, endY]( size_t y ) -> void
			{
				for ( float x = 0.0f; x < m_fWidth; ++x )
				{
					float xx = ( 2.0f * ( ( x + 0.5f ) * invWidth ) - 1.0f ) * angle * aspectratio;
					float yy = ( 1.0f - 2.0f * ( ( y + 0.5f ) * invHeight ) ) * angle;
					Vec3f raydir( xx, yy, -1.0f );
					raydir.normalize();
					currentChunk[(int)m_fWidth * ( y - startY ) + (int)x] = m_rayTracer.Trace( Vec3f( 0.0f ), raydir, spheres, 0u, sphereCount );
				}
			} );
#else
			unsigned index = 0u;
			for ( float y = (float)startY; y < endY; ++y )
			{
				for ( float x = 0.0f; x < m_fWidth; ++x, index++ )
				{
					float xx = ( 2.0f * ( ( x + 0.5f ) * invWidth ) - 1.0f ) * angle * aspectratio;
					float yy = ( 1.0f - 2.0f * ( ( y + 0.5f ) * invHeight ) ) * angle;
					Vec3f raydir( xx, yy, -1.0f );
					raydir.normalize();
					currentChunk[index] = m_rayTracer.Trace( Vec3f( 0.0f ), raydir, spheres, 0u, 4u );
				}
			}
#endif
			int charIndex = 0;
			for ( unsigned i = 0u; i < ( m_fWidth * m_fHeight ) / THREAD_COUNT; ++i )
			{
				currentArr[charIndex] = (unsigned char)( ( 1.0f < currentChunk[i].x ? 1.0f : currentChunk[i].x ) * 255 );
				currentArr[charIndex + 1] = (unsigned char)( ( 1.0f < currentChunk[i].y ? 1.0f : currentChunk[i].y ) * 255 );
				currentArr[charIndex + 2] = (unsigned char)( ( 1.0f < currentChunk[i].z ? 1.0f : currentChunk[i].z ) * 255 );
				charIndex += 3;
			}
		} );
		startY += (int)m_fHeight / THREAD_COUNT;
		endY += (int)m_fHeight / THREAD_COUNT;
	}
	ThreadManager::WaitForAllThreads();

	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::string filepath = "./Resources/Spheres/";
	if ( !std::filesystem::exists( filepath ) )
		std::filesystem::create_directory( filepath );

	std::stringstream ss;
	ss << filepath << "spheres" << std::to_string( iteration ) << ".ppm";
	std::string tempString = ss.str();
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs( filename, std::ios::out | std::ios::binary );
	ofs << "P6\n" << m_fWidth << " " << m_fHeight << "\n255\n";
	for ( unsigned i = 0u; i < THREAD_COUNT; i++ )
	{
		ofs.write( charArrs[i], std::streamsize( ( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * 3 ) );
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
	charArrs = nullptr;
	chunkArrs = nullptr;
}
#pragma endregion

#ifdef MEMORY_POOLS
void Renderer::CreatePools( Heap* pChunkHeap, Heap* pCharHeap )
{
	m_pChunkPool = new( pChunkHeap ) MemoryPool( pChunkHeap, THREAD_COUNT, unsigned( ( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * sizeof( Vec3f ) ) );
	m_pCharPool = new( pCharHeap ) MemoryPool( pCharHeap, THREAD_COUNT, unsigned( ( ( m_fWidth * m_fHeight ) / THREAD_COUNT ) * 3 ) );
}

void Renderer::DeletePools()
{
	delete m_pChunkPool;
	m_pChunkPool = nullptr;

	delete m_pCharPool;
	m_pCharPool = nullptr;
}
#endif