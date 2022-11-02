#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer() : lastTime( std::chrono::steady_clock::now() ) {}
	float Mark() noexcept;
	float Peek() noexcept;
private:
	std::chrono::steady_clock::time_point lastTime;
};

#endif