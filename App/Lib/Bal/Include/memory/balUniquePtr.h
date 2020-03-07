/*!
 * @file   balUniquePtr.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <memory>
// Heap
#include <heap/balHeapBlockBase.h>

// ----------------------------------------------------------------------------
namespace bal {

template <class T, class D = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, D>;

template <class T, class... Args, std::enable_if_t<!std::is_array_v<T>, int> = 0>
UniquePtr<T> make_unique(heap::BlockBase* p_heap, Args&&... args)
{
    void* ptr = p_heap->alloc(sizeof(T));
    return UniquePtr<T>(new (ptr) T(std::forward<Args>(args)...));
}

}
