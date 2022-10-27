#pragma once
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Heap.h"
#define DEADCODE 0xDEADC0DE

struct Header
{
	int size;
#ifdef _DEBUG
	int check = DEADCODE;
#endif
	Heap* pHeap = nullptr;
	Header* pPrevious = nullptr;
	Header* pNext = nullptr;
};

struct Footer
{
	int reserved;
#ifdef _DEBUG
	int check = DEADCODE;
#endif
};

void* operator new ( size_t size );
void* operator new ( size_t size, Heap* heap );
void operator delete ( void* pMem );

#endif