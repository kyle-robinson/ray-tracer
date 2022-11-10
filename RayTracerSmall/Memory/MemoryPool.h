#pragma once
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "MemoryManager.h"

class MemoryPool
{
public:
	MemoryPool( Heap* heap, unsigned noOfChunks, unsigned sizeOfChunks );
	~MemoryPool();
	void* Allocate( unsigned requestedBytes );
	void Free( void* pValue );
private:
	// Doubly linked list
	struct Node
	{
		Node* pPrev;
		Node* pNext;
	};

	// Linked lists to manage memory blocks
	Node* m_pAllocatedMemBlock = nullptr;
	Node* m_pFreeMemBlock = nullptr;

	void* m_pMemBlock = nullptr; // Start of pool
	unsigned m_lBlockSize; // Size of 1 memory block
	unsigned m_lPoolSize; // Total size of pool
};

#endif