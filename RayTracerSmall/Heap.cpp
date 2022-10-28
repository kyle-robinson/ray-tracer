#include "Heap.h"
#include "MemoryManager.h"

void* Heap::operator new ( size_t size )
{
	// Allocate memory block
	return (void*)malloc( size );
}

void Heap::AllocateMemory( Header* header, int size )
{
	// Update memory allocation data
	totalMemoryAllocated += size;
	if ( totalMemoryAllocated > peakMemoryAllocated )
		peakMemoryAllocated = totalMemoryAllocated;

	header->size = size;
#ifdef _DEBUG
	header->check = DEADCODE;
#endif

	// Setup ptrs in doubly linked list
	header->pPrevious = nullptr;
	header->pNext = pHeader;
	if ( pHeader != nullptr )
		pHeader->pPrevious = header;
	pHeader = header;
}

void Heap::DeallocateMemory( Header* header, int size )
{
	totalMemoryAllocated -= size;

	// Remove from doubly linked list
	if ( pHeader == header )
		pHeader = header->pNext;
	
	if ( header->pNext != nullptr )
		header->pNext->pPrevious = header->pPrevious;

	if ( header->pPrevious != nullptr )
		header->pPrevious->pNext = header->pNext;
}

void Heap::PrintHeapInformation()
{
	std::cout << "Printing heap information...\n\n";
	std::cout << "Total Memory:\t" << totalMemoryAllocated << '\n';
	std::cout << "Peak Memory:\t" << peakMemoryAllocated << '\n';

	if ( pHeader != nullptr )
	{
		std::cout << "\n| Address |\t\t\t| Type |\t\t| Size |\n";

		// Iterate through each element in the heap
		size_t size = sizeof( Header );
		Header* pCurrentHeader = pHeader;
		while ( pCurrentHeader != nullptr )
		{
			auto& start = *( pCurrentHeader + size );
			std::cout << &start << "\t\t" << typeid( start ).name() << "\t\t" << pCurrentHeader->size << '\n';

			if ( pCurrentHeader->pNext == nullptr )
				break;
			
			pCurrentHeader = pCurrentHeader->pNext;
		}
	}

	std::cout << "\n\n";
}

void Heap::CheckHeapIntegrity()
{
#ifdef _DEBUG
	std::cout << "Checking heap integrity...\n\n";
	
	int errorCount = 0;
	bool foundError = false;
	if ( pHeader != nullptr )
	{
		// Iterate through memory blocks
		Header* pCurrentHeader = pHeader;
		while ( pCurrentHeader != nullptr )
		{
			// Check integrity of header
			if ( pCurrentHeader->check != DEADCODE )
			{
				errorCount++;
				foundError = true;
				std::cout << "[ERROR] Heap::CheckIntegrity\n";
				std::cout << "Header check code does not match!\n";
			}

			// Check integrity of footer
			void* pFooterAddress = ( (char*)pCurrentHeader + sizeof( Header ) + pCurrentHeader->size );
			Footer* pFooter = (Footer*)pFooterAddress;
			if ( pFooter->check != DEADCODE )
			{
				errorCount++;
				foundError = true;
				std::cout << "[ERROR] Heap::CheckIntegrity\n";
				std::cout << "Footer check code does not match!\n";
			}

			pCurrentHeader = pCurrentHeader->pNext;
		}
	}

	// Print error information
	if ( foundError )
	{
		std::cout << "[ERROR] Heap::CheckIntegrity\n";
		std::cout << "Error count:\t" << errorCount;
	}
	else
	{
		std::cout << "[INFO] Heap::CheckIntegrity\n";
		std::cout << "[INFO] No errors found in heap:\t" << heapName;
	}

	std::cout << "\n\n---------------------------------\n\n";
#endif
}