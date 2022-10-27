#pragma once
#ifndef HEAD_H
#define HEAD_H

struct Header;
#include <iostream>

class Heap
{
public:
	Heap( std::string heapName ) { this->heapName = heapName; }
	void* operator new ( size_t size );
	
	void AllocateMemory( Header* header, int size );
	void DeallocateMemory( Header* header, int size );

	void CheckHeapIntegrity();
	void PrintHeapInformation();

	inline Header* GetHeader() const noexcept { return pHeader; }
	inline std::string GetName() const noexcept { return heapName; }
	inline int GetTotalAllocated() const noexcept { return totalMemoryAllocated; }
	inline int GetPeakMemoryAllocated() const noexcept { return peakMemoryAllocated; }
private:
	std::string heapName;
	Header* pHeader = nullptr;
	int peakMemoryAllocated = 0;
	int totalMemoryAllocated = 0;
};

#endif