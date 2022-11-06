#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#if defined _WIN32
#include <thread>
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <vector>
#include <functional>

class ThreadManager
{
public:
	static void CreateThread( std::function<void()> thread);
	static void WaitForAllThreads();
private:
#if defined _WIN32
	static std::vector<std::thread*> m_pThreads;
#else
	static std::vector<pid_t> m_pThreads;
#endif
};

#endif