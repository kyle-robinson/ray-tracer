#include "Console.h"

#ifndef __linux__
#include <Windows.h>
#endif

void Console::SetColor( Color color ) noexcept
{
#ifdef _WIN32
	HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );
	int colorCode = 0;
	switch ( color )
	{
	case Color::WHITE:	 colorCode = 15; break;
	case Color::RED:	 colorCode = 4;  break;
	case Color::GREEN:	 colorCode = 2;  break;
	case Color::YELLOW:  colorCode = 14; break;
	case Color::BLUE:	 colorCode = 1;  break;
	case Color::MAGENTA: colorCode = 5;  break;
	case Color::CYAN:	 colorCode = 3;  break;
	}
	SetConsoleTextAttribute( console, colorCode );
#else
	switch ( color )
	{
	case Color::WHITE:	 printf( "\033[0m"  ) break;
	case Color::RED:	 printf( "\033[31m" ) break;
	case Color::GREEN:	 printf( "\033[32m" ) break;
	case Color::YELLOW:  printf( "\033[33m" ) break;
	case Color::BLUE:	 printf( "\033[34m" ) break;
	case Color::MAGENTA: printf( "\033[35m" ) break;
	case Color::CYAN:	 printf( "\033[36m" ) break;
	}
#endif
}