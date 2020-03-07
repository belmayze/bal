/*!
 * @file   balHeapBlock.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBlock.h>

namespace bal::heap {

// ----------------------------------------------------------------------------

std::unique_ptr<Block> Block::Create(std::size_t size, const StringPtr& name, BlockBase* p_parent)
{
    std::unique_ptr<std::uint8_t[]> p_buf;
    p_buf = std::make_unique<std::uint8_t[]>(size);
    std::unique_ptr<Block> p_heap = std::unique_ptr<Block>(new Block(name));
    p_heap->mpRootPtr = std::move(p_buf);
    return p_heap;
}

// ----------------------------------------------------------------------------

void* Block::allocImpl_(std::size_t size)
{
    return nullptr;
}

// ----------------------------------------------------------------------------

}
