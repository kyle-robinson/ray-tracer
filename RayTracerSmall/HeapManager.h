#pragma once
#ifndef HEAPMANAGER_H
#define HEAPMANAGER_H

#include "Heap.h"
#include <unordered_map>

class HeapManager
{
public:
	inline static Heap& GetDefaultHeap() noexcept { return defaultHeap; };
	inline static Heap* GetHeap( std::string heapName ) noexcept { return heaps[heapName]; }
	
	static Heap* CreateHeap( std::string heapName );
	static void DeleteHeaps();
	static void CheckAllHeaps();
private:
	static Heap defaultHeap;
	static std::unordered_map<std::string, Heap*> heaps;
};

#endif