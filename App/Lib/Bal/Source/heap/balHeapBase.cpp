/*!
 * @file   balHeapBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBase.h>
#include <heap/balHeapManager.h>
#include <math/balMath.h>

namespace bal {

void* HeapBase::alloc(std::size_t size, std::size_t alignment)
{
    std::size_t alignmented_size = Math<std::size_t>::Ceil(size, alignment);
    return allocImpl_(alignmented_size);
}

}

/*!
 * ヒープ指定なし
 */
void* operator new(std::size_t size)
{
    bal::HeapBase* p_current_heap = bal::HeapManager::GetInstance().getCurrentHeap();
    return p_current_heap->alloc(size);
}
