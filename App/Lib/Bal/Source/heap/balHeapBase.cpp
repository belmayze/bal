/*!
 * @file   balHeapBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBase.h>
#include <math/balMath.h>

namespace bal {

void* HeapBase::alloc(std::size_t size, std::size_t alignment)
{
    std::size_t alignmented_size = Math<std::size_t>::Ceil(size, alignment);
    return allocImpl_(alignmented_size);
}

}
