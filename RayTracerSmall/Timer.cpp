#include "Timer.h"

float Timer::Mark() noexcept
{
	const auto oldTime = lastTime;
	lastTime = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = lastTime - oldTime;
	return frameTime.count();
}

float Timer::Peek() noexcept
{
	return std::chrono::duration<float>( std::chrono::steady_clock::now() - lastTime ).count();
}