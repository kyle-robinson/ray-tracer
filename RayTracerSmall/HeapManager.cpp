#include "HeapManager.h"

Heap* HeapManager::CreateHeap( std::string heapName )
{
	Heap* heap = new Heap( heapName );
	heaps.insert( { heapName, heap } );
	return heap;
}

void HeapManager::DeleteHeaps()
{
	heaps.clear();
}

void HeapManager::CheckAllHeaps()
{
	// Display data and check integrity for all stored heaps
	std::unordered_map<std::string, Heap*>::const_iterator it;
	for ( it = heaps.begin(); it != heaps.end(); ++it )
	{
		it->second->PrintHeapInformation();
		it->second->CheckHeapIntegrity();
	}

	// Also check for the default heap
	defaultHeap.PrintHeapInformation();
	defaultHeap.CheckHeapIntegrity();
}

Heap HeapManager::defaultHeap = Heap( "Default" );
std::unordered_map<std::string, Heap*> HeapManager::heaps;