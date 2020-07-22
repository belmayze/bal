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

Container::Container(uint32_t size)
{
    // 先頭にタグを作る
    uint8_t* ptr = reinterpret_cast<uint8_t*>(this);
    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(ptr);
    BackwardTag* p_backward_tag = reinterpret_cast<BackwardTag*>(ptr + cForwardTagSize + size);

    // タグ書き込み
    p_forward_tag->mAllocSize = size;

    p_backward_tag->mAllSize = size + static_cast<uint32_t>(cForwardTagSize) + static_cast<uint32_t>(cBackwardTagSize);
}

// ----------------------------------------------------------------------------

bool Container::isFree() const
{
    const ForwardTag* p_forward_tag = reinterpret_cast<const ForwardTag*>(this);
    return (p_forward_tag->mAllocSize & 0x80000000) == 0;
}

// ----------------------------------------------------------------------------

void Container::setFree(bool free)
{
    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(this);

    if (!free)
    {
        p_forward_tag->mAllocSize = p_forward_tag->mAllocSize | 0x80000000;
    }
    else
    {
        p_forward_tag->mAllocSize = p_forward_tag->mAllocSize & 0x7FFFFFFF;
    }
}

// ----------------------------------------------------------------------------

Container* Container::split(uint32_t size)
{
    // 使用中なら分割できない
    BAL_ASSERT(isFree());

    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(this);
    // 分割できるかチェック（データ容量とタグ、分割先のタグの合計以上であれば分割できる）
    if (size < (p_forward_tag->mAllocSize + cAllTagSize * 2))
    {
        // 分割した容量計算
        uint32_t next_size = p_forward_tag->mAllocSize - size - cAllTagSize;

        // 今のタグを書き換える
        new (this) Container(size);

        // 分割先のタグを生成する
        uint8_t* ptr_next = reinterpret_cast<uint8_t*>(this + p_forward_tag->mAllocSize + cAllTagSize);
        new (ptr_next) Container(next_size);

        return this;
    }

    // 分割できなかったら nullptr を返す
    return nullptr;
}

// ----------------------------------------------------------------------------

void Container::merge()
{
    // 使用中なら統合できない
    BAL_ASSERT(isFree());

    // 結合関数
    auto merge_func = [](Container* p_prev, Container* p_next)
    {
        uint32_t merged_size = p_prev->getSize() + p_next->getAllSize();
        new (p_prev) Container(merged_size);
    };

    BackwardTag* p_backward_tag = reinterpret_cast<BackwardTag*>(reinterpret_cast<uint8_t*>(this) - cBackwardTagSize);
    if (p_backward_tag->mAllSize != 0 && getPrev()->isFree())
    {
        // 末端でなければ結合
        merge_func(getPrev(), this);
    }
    
    ForwardTag* p_forward_tag = reinterpret_cast<ForwardTag*>(reinterpret_cast<uint8_t*>(this) + getAllSize());
    if (p_forward_tag->mAllocSize != 0 && getNext()->isFree())
    {
        // 末端でなければ結合
        merge_func(this, getNext());
    }
}

// ----------------------------------------------------------------------------

void* Container::getData()
{
    return reinterpret_cast<uint8_t*>(this) + cForwardTagSize;
}

// ----------------------------------------------------------------------------

uint32_t Container::getSize() const
{
    const ForwardTag* p_forward_tag = reinterpret_cast<const ForwardTag*>(this);
    return (p_forward_tag->mAllocSize & 0x7FFFFFFF);
}

// ----------------------------------------------------------------------------

uint32_t Container::getAllSize() const
{
    const BackwardTag* p_backward_tag = reinterpret_cast<const BackwardTag*>(reinterpret_cast<const uint8_t*>(this) + getSize() + cForwardTagSize);
    return p_backward_tag->mAllSize;
}

// ----------------------------------------------------------------------------

Container* Container::getNext()
{
    return reinterpret_cast<Container*>(reinterpret_cast<uint8_t*>(this) + getSize() + cAllTagSize);
}

// ----------------------------------------------------------------------------

Container* Container::getPrev()
{
    BackwardTag* p_backward_tag = reinterpret_cast<BackwardTag*>(reinterpret_cast<uint8_t*>(this) - cBackwardTagSize);
    return reinterpret_cast<Container*>(reinterpret_cast<uint8_t*>(this) - p_backward_tag->mAllSize);
}

// ----------------------------------------------------------------------------

void Container::setFill(uint8_t data)
{
#if BAL_BUILD_UNDER_DEVELOPMENT
    std::memset(getData(), (data << 24 | data << 16 | data << 8 | data), getSize());
#endif // BAL_BUILD_UNDER_DEVELOPMENT
}

// ----------------------------------------------------------------------------

}
