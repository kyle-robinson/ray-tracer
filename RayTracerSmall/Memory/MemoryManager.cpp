#include "MemoryManager.h"
#include "HeapManager.h"
#include "../Utility/Console.h"
#include <iostream>

void* operator new ( size_t size )
{
	// Provide a heap if one was not already
	return ::operator new ( size, &HeapManager::GetDefaultHeap() );
}

void* operator new ( size_t size, Heap* heap )
{
	// Calculate and reserve required bytes
	size_t requestedBytes = size + sizeof( Header ) + sizeof( Footer );
	char* pMem = (char*)malloc( requestedBytes );

	// Allocate memory for the heap
	Header* pHeader = (Header*)pMem;
	pHeader->pHeap = heap;
	heap->AllocateMemory( pHeader, size );

	// Get the location for the footer start position
	void* pFooterAddress = pMem + sizeof( Header ) + size;
	Footer* pFooter = (Footer*)pFooterAddress;
#ifdef _DEBUG
	pFooter->check = DEADCODE;
#endif

	// Get the start location of the usable memory block
	void* pMemStartBlock = pMem + sizeof( Header );
	return pMemStartBlock;
}

void operator delete ( void* pMem )
{
	// Get the memory location of the header
	Header* pHeader = (Header*)( (char*)pMem - sizeof( Header ) );

#ifdef _DEBUG
	if ( pHeader->check != DEADCODE )
	{
		Console::SetColor( Console::Color::RED );
		std::cout << "[ERROR] MemoryManager::delete\n";
		std::cout << "Header check does not match!\n";
	}

	// Get the memory location of the footer
	Footer* pFooter = (Footer*)( (char*)pMem + pHeader->size );
	if ( pFooter->check != DEADCODE )
	{
		Console::SetColor( Console::Color::RED );
		std::cout << "[ERROR] MemoryManager::delete\n";
		std::cout << "Footer check does not match!\n";
	}
	Console::SetColor( Console::Color::WHITE );
#endif

	// Deallocate memory from the heap
	pHeader->pHeap->DeallocateMemory( pHeader, pHeader->size );
	free( pHeader );
}