#include "MemoryPool.h"

MemoryPool::MemoryPool( Heap* heap, uint32_t noOfChunks, uint32_t sizeOfChunks ) :
	m_lPoolSize( noOfChunks* ( sizeOfChunks + sizeof( Node ) ) ),
	m_lBlockSize( sizeOfChunks )
{
	size_t requestedBytes = m_lPoolSize + sizeof( Header ) + sizeof( Footer ); // Required bytes
	char* pMem = (char*)malloc( requestedBytes ); // Allocated memory

	Header* pHeader = (Header*)pMem;
	pHeader->pHeap = heap;

	// Allocate memory to the heap counter and insert poll into linked list
	heap->AllocateMemory( pHeader, m_lPoolSize );
	m_pMemBlock = pMem + sizeof( Header );

	if ( m_pMemBlock )
	{
		for ( uint32_t i = 0u; i < noOfChunks; i++ )
		{
			// Linked list of memory blocks
			Node* pCurrentNode = (Node*)( (char*)m_pMemBlock + i * ( sizeOfChunks + sizeof( Node ) ) );

			// Set default values
			pCurrentNode->pPrev = nullptr;
			pCurrentNode->pNext = m_pFreeMemBlock;

			if ( m_pFreeMemBlock != nullptr )
				m_pFreeMemBlock->pPrev = pCurrentNode; // Set previous node to current node
			m_pFreeMemBlock = pCurrentNode;
		}
	}

	void* pFooterAddr = ( (char*)pMem + sizeof( Header ) + m_lPoolSize );
	Footer* pFooter = (Footer*)pFooterAddr;
#ifdef _DEBUG
	pFooter->check = DEADCODE;
#endif
}

MemoryPool::~MemoryPool()
{
	delete m_pMemBlock;
}

// Allocate memory if memory pool cannot any
void* MemoryPool::Allocate( uint32_t requestedBytes )
{
	Node* pCurUnit = m_pFreeMemBlock;

	// Get a unit from "free linked list"
	m_pFreeMemBlock = pCurUnit->pNext;
	if ( m_pFreeMemBlock != nullptr )
		m_pFreeMemBlock->pPrev = nullptr;
	pCurUnit->pNext = m_pAllocatedMemBlock;

	if ( m_pAllocatedMemBlock != nullptr )
		m_pAllocatedMemBlock->pPrev = pCurUnit;
	m_pAllocatedMemBlock = pCurUnit;

	return (void*)( (char*)pCurUnit + sizeof( Node ) );
}

//  If the void pointer points to a memory unit, add it to "free linked list"
void MemoryPool::Free( void* pValue )
{
	// Check that the memory to delete is in the pool
	if ( pValue > m_pMemBlock && pValue < ( (char*)m_pMemBlock + m_lPoolSize ) )
	{
		Node* pCurUnit = (Node*)( (char*)pValue - sizeof( Node ) );
		Node* pPrev = pCurUnit->pPrev;
		Node* pNext = pCurUnit->pNext;

		if ( pPrev == nullptr )
			m_pAllocatedMemBlock = pCurUnit->pNext;
		else
			pPrev->pNext = pNext;

		if ( pNext != nullptr )
			pNext->pPrev = pPrev;

		pCurUnit->pNext = m_pFreeMemBlock;
		if ( m_pFreeMemBlock != nullptr )
			m_pFreeMemBlock->pPrev = pCurUnit;

		m_pFreeMemBlock = pCurUnit;
	}
}