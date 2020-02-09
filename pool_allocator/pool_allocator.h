#pragma once

#include "platform_dependent.h"
#include <cstdint>

/* ������� ��������� ������
* ��������� ������ ���� � ����������� � ������������� ������� ���������
* POOL_SIZE - ������ ���� � ������
* BLOCK_SIZE - ����� � ������
*
* ������� ��������� ������ ���������� ��� �������� ���������������� ������
* ��� �������� ����� ( ����� �������� �� �������� ���� ���������� ����� ) � 
* �������� ����������: ��������� �� ��������� ���� � ������ ����� � ����� ������� ������,
* ������, ���� ���� ���������, �� � ������ ����� ����� ��������� �� ��������� ����
*         ���� ���� �����, �� � ������ ����� ������
* union Node
* {
*    Block* next;
*    std::uint8_t pool[NUMBER_OF_BLOCKS * BLOCK_SIZE];
* };
*/

/* ������������ ������,
*  � ��������� ����������� ��������� � ������������� ������ �������� � ����������, 
*  ����� BLOCK_SIZE ������ ���� �� ������ ������� ��������� �� ������� ��������� ��-�� ����, ��� ������ ����� ��������� � ���������� 
*/
constexpr size_t ALIGN = alignof( int* ); 
/* ������� ���� � ������ */
constexpr size_t POOL_SIZE = 16;
/* ������ ����� � ������ */
constexpr size_t BLOCK_SIZE = 4;
constexpr bool isEven(size_t lvalue, size_t rvalue) {
    return !( lvalue % rvalue );
}
/* ��������� ������� ��������� �� ������������ */
#define CHECK_ALIGN_BLOCK_SIZE       static_assert( isEven( BLOCK_SIZE, ALIGN ), "parameter BLOCK_SIZE is not an alignment!")
#define CHECK_ALIGN_POOL_SIZE        static_assert( isEven( POOL_SIZE, ALIGN ), "parameter POOL_SIZE is not an alignment!")
#define CHECK_NUMBER_BLOCKS_IN_POOL  static_assert( isEven( POOL_SIZE, BLOCK_SIZE ), "parameter POOL_SIZE doesn't full fit in POOL_SIZE!") 
CHECK_ALIGN_BLOCK_SIZE;
CHECK_ALIGN_POOL_SIZE;
CHECK_NUMBER_BLOCKS_IN_POOL;

/* ���������� ������ � ���� */
constexpr size_t NUMBER_OF_BLOCKS = ( POOL_SIZE / BLOCK_SIZE );

/* TODO: ���������� ����� template<size_t BLOCK_SIZE, size_t POOL_SIZE> ��� ������ �������������� */
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

    /* �������� ������ �� �������� ���������, ���������
    *  @param[in]  sizemem - size of the block to allocate
    *  @return     ���������� ��������� �� ���������� ������, � ������ ���� ���������� nullptr
    *
    *                             -----------------
    *                             |               |
    *                             |              '|'
    *      ||||||||||||||||| ||||||||||||||||||| ||||||||||||||||||
    *      | data          | |next|            | | null_ptr|      |
    *      ||||||||||||||||| ||||||||||||||||||| ||||||||||||||||||
    */
	void* malloc( size_t sizemem );

    /* ���������� ���������� ���� ����������
    *  �������� ����� ������� � ���������� �� ������ ��� �� �������� ������� ������
    *  @param[in]  *ptr - ��������� �� ����� ���������� ���� ���������� � ��������� ���������� ���������.
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