#include "Timer.h"
#include "Console.h"
#include "Renderer.h"
#include "HeapManager.h"

//[comment]
// In the main function, we will create the scene which is composed of 5 spheres
// and 1 light (which is also a sphere). Then, once the scene description is complete
// we render that scene, by calling the render() function.
//[/comment]
int main( int argc, char **argv )
{
	// Get user input for render function
	std::cout << "Please select a render function. [1-4]\n";
	std::cout << " 1. Basic Render.\n";
	std::cout << " 2. Shrinking Render.\n";
	std::cout << " 3. Smooth Scale Render.\n";
	std::cout << " 4. Json File Render.\n\n";

	char inputRender;
	bool shouldContinue = false;
	while ( shouldContinue == false )
	{
		std::cin.get( inputRender );
		if ( inputRender == '1' ||
			inputRender == '2' ||
			inputRender == '3' || 
			inputRender == '4' )
			shouldContinue = true;
		std::cout << '\n';
	};

	// Get user input for target resolution
	std::cout << "Please select a target resolution. [1-2]\n";
	std::cout << " 1. 640 x 480\n";
	std::cout << " 2. 1920 x 1080\n";

	char inputResolution;
	shouldContinue = false;
	while ( shouldContinue == false )
	{
		std::cin.get( inputResolution );
		if ( inputResolution == '1' ||
			inputResolution == '2' )
			shouldContinue = true;
		std::cout << '\n';
	};

#ifdef MEMORY_POOLS
	Heap* pChunkHeap = HeapManager::CreateHeap( "Chunk Heap" );
	Heap* pCharHeap = HeapManager::CreateHeap( "Char Heap" );
#endif

	Renderer renderer;
	switch ( inputResolution )
	{
	case '1': renderer.SetResolution( 640.0f, 480.0f ); break;
	case '2': renderer.SetResolution( 1920.0f, 1080.0f ); break;
	}
#ifdef MEMORY_POOLS
	renderer.CreatePools( pChunkHeap, pCharHeap );
#endif

	// Select render function
	Timer timer;
	switch ( inputRender )
	{
	case '1': renderer.Render_Basic(); break;
	case '2': renderer.Render_Shrinking(); break;
	case '3': renderer.Render_SmoothScaling(); break;
	case '4': renderer.Render_JsonFile( "Resources/scene.json" ); break;
	}

	// Print time taken to render
	float timeElapsed = timer.Mark();
	Console::SetColor( Console::Color::CYAN );
	std::cout << "[TIME ELAPSED]\t" << timeElapsed << " seconds\n";

#ifdef MEMORY_POOLS
	renderer.DeletePools();
#endif

	// Print heap information
	Console::SetColor( Console::Color::MAGENTA );
	std::cout << "\nHEAP INFORMATION\n";
	HeapManager::CheckAllHeaps();

	Console::SetColor( Console::Color::MAGENTA );
	std::cout << "\nDELETING HEAPS\n";
	HeapManager::DeleteHeaps();

	Console::SetColor( Console::Color::WHITE );

#ifdef MEMORY_POOLS
	pChunkHeap = nullptr;
	pCharHeap = nullptr;
#endif

	return 0;
}