/*!
 * @file   balHeap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeap.h>

namespace bal {

// ----------------------------------------------------------------------------

std::unique_ptr<Heap> Heap::Create(std::size_t size, const StringPtr& name, HeapBase* p_parent)
{
    std::unique_ptr<std::uint8_t[]> p_buf;
    p_buf = std::make_unique<std::uint8_t[]>(size);
    std::unique_ptr<Heap> p_heap = std::unique_ptr<Heap>(new Heap(name));
    p_heap->mpRootPtr = std::move(p_buf);
    return p_heap;
}

// ----------------------------------------------------------------------------

void* Heap::allocImpl_(std::size_t size)
{
    return nullptr;
}

// ----------------------------------------------------------------------------

}
