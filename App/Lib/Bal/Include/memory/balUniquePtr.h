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

template <class T, class... Args, std::enable_if_t<!std::is_array_v<T>, int> = 0>
std::unique_ptr<T> make_unique(heap::BlockBase* p_heap, Args&&... args)
{
    return std::unique_ptr<T>(new (p_heap) T(std::forward<Args>(args)...));
}

}
