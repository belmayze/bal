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

std::unique_ptr<Block> Block::Create(size_t size, const StringPtr& name, BlockBase* p_parent)
{
    // ヒープ作成
    std::unique_ptr<uint8_t[]> p_buf;
    p_buf = make_unique<uint8_t[]>(p_parent, size);
    std::unique_ptr<Block> p_heap = std::unique_ptr<Block>(new (p_buf.get()) Block(name));
    p_heap->mpRootPtr = std::move(p_buf);
    p_heap->mBackwardTag.mAllSize = 0;
    p_heap->mSize = static_cast<uint32_t>(size);

    // コンテナを作っておく
    constexpr const size_t cManagementSize = sizeof(Block);
    void* containerPtr = p_heap->mpRootPtr.get() + cManagementSize;
    Container* p_container = new (containerPtr) Container(static_cast<uint32_t>(size - cManagementSize - Container::cAllTagSize - Container::cForwardTagSize));

    // 末端がわかるようにタグを入れておく
    uint8_t* p_final_tag = reinterpret_cast<uint8_t*>(p_container) + p_container->getAllSize();
    Container::BackwardTag* p_backward_tag = reinterpret_cast<Container::BackwardTag*>(p_final_tag);
    p_backward_tag->mAllSize = 0;

    return p_heap;
}

// ----------------------------------------------------------------------------

void* Block::allocImpl_(size_t size)
{
    // フリーのコンテナを分割して利用する
    constexpr const size_t cManagementSize = sizeof(Block);
    Container* p_container = reinterpret_cast<Container*>(reinterpret_cast<uint8_t*>(this) + cManagementSize);

    // 線形検索して、空いてたら利用
    while (true)
    {
        if (p_container->isFree() && p_container->getSize() >= size)
        {
            // サイズに余裕があれば分割して利用する
            Container* p_target = p_container->split(static_cast<uint32_t>(size));
            if (!p_target)
            {
                p_target = p_container;
            }
            p_target->setFree(false);
            p_target->setFill(0xAA);
            return p_target->getData();
        }
        else
        {
            p_container = p_container->getNext();
            // ヒープ領域超えたらアウト
            uint8_t* p_limit_ptr = reinterpret_cast<uint8_t*>(this) + mSize;
            if (reinterpret_cast<uintptr_t>(p_container) >= reinterpret_cast<uintptr_t>(p_limit_ptr))
            {
                BAL_ASSERT(0);
                return nullptr;
            }
        }
    }

    return nullptr;
}

// ----------------------------------------------------------------------------

}
