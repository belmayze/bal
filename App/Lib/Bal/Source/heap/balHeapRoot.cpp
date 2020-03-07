/*!
 * @file   balHeapRoot.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapRoot.h>

namespace bal {

// ----------------------------------------------------------------------------

void HeapRoot::Deleter::operator()(void* ptr) const
{
    free(ptr);
}

// ----------------------------------------------------------------------------

UniquePtr<HeapRoot, HeapRoot::Deleter> HeapRoot::Create(std::size_t size)
{
    // ルートヒープは OS から確保
    void* ptr = malloc(size);
    UniquePtr<HeapRoot, Deleter> p_heap = UniquePtr<HeapRoot, Deleter>(new (ptr) HeapRoot("RootHeap"));
    return p_heap;
}

// ----------------------------------------------------------------------------

void* HeapRoot::allocImpl_(std::size_t size)
{
    return nullptr;
}

// ----------------------------------------------------------------------------

}
