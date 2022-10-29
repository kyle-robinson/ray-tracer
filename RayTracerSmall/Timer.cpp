#include "Timer.h"
#include <cmath>

float Timer::Mark() noexcept
{
	const auto oldTime = lastTime;
	lastTime = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = lastTime - oldTime;
	
	int decimalPlaces = 2;
	const float multiplier = std::pow( 10.0f, decimalPlaces );
	return std::ceil( frameTime.count() * multiplier ) / multiplier;
}

float Timer::Peek() noexcept
{
	return std::chrono::duration<float>( std::chrono::steady_clock::now() - lastTime ).count();
}