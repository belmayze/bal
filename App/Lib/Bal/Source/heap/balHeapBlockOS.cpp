/*!
 * @file   balHeapBlockOS.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// C++
#include <memory>
// bal
#include <heap/balHeapBlockOS.h>

namespace bal::heap {

// ----------------------------------------------------------------------------

std::unique_ptr<BlockOS> BlockOS::Create(const StringPtr& name)
{
    BlockOS* ptr = new BlockOS(name);
    return std::unique_ptr<BlockOS>(ptr);
}

// ----------------------------------------------------------------------------

void* BlockOS::allocImpl_(std::size_t size)
{
    return malloc(size);
}

// ----------------------------------------------------------------------------

}
