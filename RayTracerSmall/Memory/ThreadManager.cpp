#include "ThreadManager.h"

#if defined _WIN32
std::vector<std::thread*> ThreadManager::m_pThreads;
#else 
std::vector<pid_t> ThreadManager::m_pThreads;
#endif

void ThreadManager::CreateThread( std::function<void()> thread )
{
#if defined _WIN32
	std::thread* newThread = new std::thread( thread );
	m_pThreads.emplace_back( newThread );
#else
	pid_t newThread = vfork();
	if ( newThread < 0 )
	{
		printf( "[ERROR] ThreadManager::CreateThread\n" );
		printf( "Failed to create fork!\n" );
	}
	else if ( newThread == 0 )
	{
		m_pThreads.emplace_back( newThread );
		task();
		_exit( 0 );
	}
	else {

	}
#endif
}

void ThreadManager::WaitForAllThreads()
{
#ifdef _WIN32
	for ( auto& thread : m_pThreads )
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
	m_pThreads.clear();
#else
	int status;
	for ( auto& thread : m_pThreads )
	{
		if ( wait( &status ) == -1 ) {}
		else if ( WIFEXITED( status ) ) {}
	}
	m_pThreads.clear();
#endif
}