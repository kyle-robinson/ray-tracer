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
#ifdef MEMORY_POOLS
	Heap* pChunkHeap = HeapManager::CreateHeap( "Chunk Heap" );
	Heap* pCharHeap = HeapManager::CreateHeap( "Char Heap" );
#endif

	Renderer renderer;
#ifdef MEMORY_POOLS
	renderer.CreatePools( pChunkHeap, pCharHeap );
#endif

	char userInput;
	bool shouldContinue = false;
	do
	{
		// Get user input
		std::cout << "Please select a render function. [1-4]\n";
		std::cout << " 1.\tBasic Render.\n";
		std::cout << " 2.\tShrinking Render.\n";
		std::cout << " 3.\tSmooth Scale Render.\n";
		std::cout << " 4.\tJson File Render.\n\n";

		std::cin.get( userInput );
		if ( userInput == '1' ||
			userInput == '2' ||
			userInput == '3' || 
			userInput == '4' )
			shouldContinue = true;
		std::system( "cls" );
	} while ( shouldContinue == false );

	Timer timer;
	switch ( userInput )
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