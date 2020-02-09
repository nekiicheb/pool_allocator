#pragma once

#include "platform_dependent.h"
#include <cstdint>

/* блочный аллокатор памяти
* параметры должны быть в соответсвии с выравниванием целевой платформы
* POOL_SIZE - размер пула в байтах
* BLOCK_SIZE - блока в байтах
*
* Блочный аллокатор памяти реализован как линейный однонаправленный список
* для экономии места ( иначе пришлось бы заводить флаг свободного блока ) и 
* простоты реализации: указатель на следующий блок и данные лежат в одном участке памяти,
* причем, если блок свободный, то в памяти блока лежит указатель на следующий блок
*         если блок занят, то в памяти лежат данные
* union Node
* {
*    Block* next;
*    std::uint8_t pool[NUMBER_OF_BLOCKS * BLOCK_SIZE];
* };
*/

/* выравнивание памяти,
*  в некоторых процессорах обращение к невыровненной памяти приводит к исключению, 
*  также BLOCK_SIZE должен быть не меньше размера указателя на целевой платформе из-за того, что данные лежат совместно с указателем 
*/
constexpr size_t ALIGN = alignof( int* ); 
/* размера пула в байтах */
constexpr size_t POOL_SIZE = 16;
/* размер блока в байтах */
constexpr size_t BLOCK_SIZE = 4;
constexpr bool isEven(size_t lvalue, size_t rvalue) {
    return !( lvalue % rvalue );
}
/* проверяем входные параметры на соответствие */
#define CHECK_ALIGN_BLOCK_SIZE       static_assert( isEven( BLOCK_SIZE, ALIGN ), "parameter BLOCK_SIZE is not an alignment!")
#define CHECK_ALIGN_POOL_SIZE        static_assert( isEven( POOL_SIZE, ALIGN ), "parameter POOL_SIZE is not an alignment!")
#define CHECK_NUMBER_BLOCKS_IN_POOL  static_assert( isEven( POOL_SIZE, BLOCK_SIZE ), "parameter POOL_SIZE doesn't full fit in POOL_SIZE!") 
CHECK_ALIGN_BLOCK_SIZE;
CHECK_ALIGN_POOL_SIZE;
CHECK_NUMBER_BLOCKS_IN_POOL;

/* количество блоков в пуле */
constexpr size_t NUMBER_OF_BLOCKS = ( POOL_SIZE / BLOCK_SIZE );

/* TODO: переписать через template<size_t BLOCK_SIZE, size_t POOL_SIZE> для гибкой параметризации */
class PoolAllocator
{
private:
	struct Block
	{
		Block *next;
	};
	Block* head = nullptr;

    alignas(ALIGN) std::uint8_t pool[ NUMBER_OF_BLOCKS * BLOCK_SIZE ];

public:
    /*
    *            -------------------  ----------------
    *            |                 |  |              |
    *            |                '|' |             '|'
    *      |||||||||||||||||| |||||||||||||||||| ||||||||||||||||||
    *      | next|          | | next|          | | null_ptr|      |
    *      |||||||||||||||||| |||||||||||||||||| ||||||||||||||||||
    */
    PoolAllocator();

    /* выделить память из блочного аллоктора, указатель
    *  @param[in]  sizemem - size of the block to allocate
    *  @return     возвращает указатель на выделенную память, в случае сбоя возвращает nullptr
    *
    *                             -----------------
    *                             |               |
    *                             |              '|'
    *      ||||||||||||||||| ||||||||||||||||||| ||||||||||||||||||
    *      | data          | |next|            | | null_ptr|      |
    *      ||||||||||||||||| ||||||||||||||||||| ||||||||||||||||||
    */
	void* malloc( size_t sizemem );

    /* освободить выделенный блок аллокатора
    *  запрещен вызов функции с указателем на пустой или не валидный участок памяти
    *  @param[in]  *ptr - указатель на ранее выделенный блок аллокатора с проверкой валидности указателя.
    *
    *            -------------------------------------
    *            |                                   |
    *            |                                  '|'
    *      |||||||||||||||||| |||||||||||||||||| ||||||||||||||||||
    *      | next|          | | data           | | null_ptr|      |
    *      |||||||||||||||||| |||||||||||||||||| ||||||||||||||||||
    */
	void free( void *ptr );
};