#include "pool_allocator.h"
#include "test_runner.h" /* unit test library */
#include <iostream>

//void CreateUnalignmentPoolSize();
//void CreateUnalignmentBlockSize();

void AllocateFullPool() {
    PoolAllocator pool;
    std::uint8_t* expected = nullptr;
    for( auto i = 0; i < NUMBER_OF_BLOCKS; ++i ) {
        std::uint8_t* received = static_cast<uint8_t*>( pool.malloc( BLOCK_SIZE ) );
        if( i == 0 ) {
            expected = received;
        }
        ASSERT_EQUAL( received, expected );
        expected += BLOCK_SIZE;
    }
}
void DeallocateFromFullPool() {
    PoolAllocator pool;
    std::uint8_t* expected = static_cast<uint8_t*>( pool.malloc( BLOCK_SIZE ) );
    std::uint8_t* received[ NUMBER_OF_BLOCKS ];
    pool.free( expected );
    for( auto i = 0; i < NUMBER_OF_BLOCKS; ++i ) {
        for( auto j = 0; j < i+1; ++j ){
            received[j] = static_cast<uint8_t*>( pool.malloc( BLOCK_SIZE ) );
        }
        for(auto j = 0; j < i + 1; ++j) {
            pool.free( received[ i - j ] );
        }
        ASSERT_EQUAL( received[0], expected );
    }
}

void AllocateZeroSize() {
    PoolAllocator pool;
    std::uint8_t* expected = static_cast<uint8_t*>( pool.malloc( BLOCK_SIZE ) );
    pool.free(expected);
    std::uint8_t* received = static_cast<uint8_t*>( pool.malloc( 0 ) );
    ASSERT_EQUAL( received, expected );
}

void AllocateSizeMoreBlockSize() {
    PoolAllocator pool;
    std::uint8_t* expected = nullptr;
    std::uint8_t* received = static_cast<uint8_t*>( pool.malloc( BLOCK_SIZE + 1 ) );
    ASSERT_EQUAL(received, expected);
}

void AllocateFromOverflowPool() {
    PoolAllocator pool;
    std::uint8_t* expected = nullptr;
    std::uint8_t* received;
    for (auto i = 0; i < NUMBER_OF_BLOCKS + 1; ++i) {
        received = static_cast<uint8_t*>(pool.malloc(BLOCK_SIZE));
    }
    ASSERT_EQUAL(received, expected);
}

#include <future>
void MultipleThreadsAllocate() {
    PoolAllocator pool;
    auto thread1 = async([&pool] {
        for (auto i = 0; i < 100000; ++i) {
            std::uint8_t* received = static_cast<uint8_t*>(pool.malloc(BLOCK_SIZE));
            if (received != nullptr) {
                pool.free(received);
            }
        }
        }
    );
    auto thread2 = async([&pool] {
        for (auto i = 0; i < 100000; ++i) {
            std::uint8_t* received = static_cast<uint8_t*>(pool.malloc(BLOCK_SIZE));
            if (received != nullptr) {
                pool.free(received);
            }
        }
        }
    );
    auto thread3 = async([&pool] {
        for (auto i = 0; i < 100000; ++i) {
            std::uint8_t* received = static_cast<uint8_t*>(pool.malloc(BLOCK_SIZE));
            if (received != nullptr) {
                pool.free(received);
            }   
        }
        }
    );
    ASSERT_EQUAL( true, true );
}

int main() {
    
    TestRunner tr;
    RUN_TEST( tr, AllocateFullPool);
    RUN_TEST(tr, DeallocateFromFullPool);
    RUN_TEST(tr, AllocateSizeMoreBlockSize);
    RUN_TEST(tr, AllocateZeroSize);
    RUN_TEST(tr, AllocateFromOverflowPool);
    RUN_TEST(tr, MultipleThreadsAllocate);
    return 0;
}

