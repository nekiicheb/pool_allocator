#pragma once

/* обработка исключений */
#define IS_SMALL_SIZE_MEMORY 1
#if( !IS_SMALL_SIZE_MEMORY )
    #include <exception>
	#define THROW_LENGTH_ERROR ( throw std::length_error() )
	#define THROW_BAD_ALLOC    ( throw std::bad_alloc() )
    #define THROW_BAD_ARGUMENT ( throw std::invalid_argument() )
#else
	#include <cassert>
	#define THROW_LENGTH_ERROR ( assert("THROW_LENGTH_ERROR") )
	#define THROW_BAD_ALLOC    ( assert("THROW_BAD_ALLOC") )
	#define THROW_BAD_ARGUMENT ( assert("THROW_BAD_ARGUMENT") )
#endif

#include <mutex>
#define MUTEX_CREATE( name ) std::mutex name
#define MUTEX_LOCK_RAII( name ) std::lock_guard<std::mutex> lock(name)
#define MUTEX_LOCK( name ) name.lock();
#define MUTEX_UNLOCK( name ) name.unlock();
