/*!
 * @file   balHeapContainer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapContainer.h>

namespace bal::heap {

// ----------------------------------------------------------------------------

Container::Container(std::uint32_t size)
{
    // 先頭にタグを作る
    std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(this);
    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(ptr);
    BackwardTag* p_backward_tag = reinterpret_cast<BackwardTag*>(ptr + cForwardTagSize + size);

    // タグ書き込み
    p_forward_tag->mAllocSize = size;

    p_backward_tag->mAllSize = size + static_cast<std::uint32_t>(cForwardTagSize) + static_cast<std::uint32_t>(cBackwardTagSize);
}

// ----------------------------------------------------------------------------

bool Container::isFree() const
{
    const ForwardTag* p_forward_tag = reinterpret_cast<const ForwardTag*>(this);
    return (p_forward_tag->mAllocSize & 0x80000000) == 0;
}

// ----------------------------------------------------------------------------

Container* Container::getNext()
{
    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(this);
    return reinterpret_cast<Container*>(reinterpret_cast<std::uint8_t*>(this) + p_forward_tag->mAllocSize + cAllTagSize);
}

// ----------------------------------------------------------------------------

Container* Container::getPrev()
{
    BackwardTag* p_backward_tag = reinterpret_cast<BackwardTag*>(reinterpret_cast<std::uint8_t*>(this) - cBackwardTagSize);
    return reinterpret_cast<Container*>(reinterpret_cast<std::uint8_t*>(this) - p_backward_tag->mAllSize);
}

// ----------------------------------------------------------------------------

}
