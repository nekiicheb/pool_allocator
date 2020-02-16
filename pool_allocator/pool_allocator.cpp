#include "pool_allocator.h"
#include <stdlib.h>

PoolAllocator::PoolAllocator(){
	Block* tail;
	head = tail = reinterpret_cast<Block*>(pool);
	/* создание линейного однонаправленного списка */
	for( int i = 0; i < NUMBER_OF_BLOCKS - 1; ++i )
	{
		tail->next = reinterpret_cast<Block*>( reinterpret_cast<std::uint8_t*>(tail) + BLOCK_SIZE ); 
		tail = tail->next; 
	}
	tail->next = nullptr; 
}

void* PoolAllocator::malloc(const size_t sizemem)
{
	if( sizemem > BLOCK_SIZE ) {
		THROW_LENGTH_ERROR;
		return nullptr;
	}
	MUTEX_LOCK_RAII(headMutex);
	if( head == nullptr ) {
		/* нет свободных блоков */
		THROW_BAD_ALLOC;
		return nullptr;
	}
	Block* freeBlock;
	freeBlock = head;
	head = head->next;
	return freeBlock;
}

void PoolAllocator::free(void* ptr)
{
	MUTEX_LOCK_RAII(headMutex);
	reinterpret_cast<Block*>(ptr)->next = head;
	head = reinterpret_cast<Block*>(ptr);
}
