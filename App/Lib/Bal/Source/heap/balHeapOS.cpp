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

UniquePtr<HeapOS> HeapOS::Create(const StringPtr& name)
{
    HeapOS* ptr = new HeapOS(name);
    return std::unique_ptr<HeapOS>(ptr);
}

// ----------------------------------------------------------------------------

void* HeapOS::allocImpl_(std::size_t size)
{
    return malloc(size);
}

// ----------------------------------------------------------------------------

}
