/*!
 * @file   balHeapOS.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// C++
#include <memory>
// bal
#include <heap/balHeapOS.h>

namespace bal {

// ----------------------------------------------------------------------------

UniquePtr<HeapOS> HeapOS::Create()
{
    return std::make_unique<HeapOS>();
}

// ----------------------------------------------------------------------------

void* HeapOS::allocImpl_(std::size_t size)
{
    return malloc(size);
}

// ----------------------------------------------------------------------------

}
