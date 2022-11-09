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
	// This sample only allows one choice per program execution. Feel free to improve upon this
	srand( 13 );
	Timer timer;
	Renderer renderer;

	//renderer.Render_Basic();
	//renderer.Render_Shrinking();
	renderer.Render_SmoothScaling();

	// Print time taken to render
	Console::SetColor( Console::Color::CYAN );
	float timeElapsed = timer.Mark();
	std::cout << "[TIME ELAPSED]\t" << timeElapsed << " seconds\n";

	// Print heap information
	Console::SetColor( Console::Color::MAGENTA );
	std::cout << "\nHEAP INFORMATION\n";
	HeapManager::CheckAllHeaps();

	Console::SetColor( Console::Color::MAGENTA );
	std::cout << "\nDELETING HEAPS\n";
	HeapManager::DeleteHeaps();

	Console::SetColor( Console::Color::WHITE );
	return 0;
}