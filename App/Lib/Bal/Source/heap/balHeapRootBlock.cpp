/*!
 * @file   balHeapRootBlock.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapContainer.h>
#include <heap/balHeapRootBlock.h>

namespace bal::heap {

// ----------------------------------------------------------------------------

void RootBlock::Deleter::operator()(void* ptr) const
{
    free(ptr);
}

// ----------------------------------------------------------------------------

std::unique_ptr<RootBlock, RootBlock::Deleter> RootBlock::Create(std::size_t size)
{
    // ルートヒープは OS から確保
    void* ptr = malloc(size);
    std::unique_ptr<RootBlock, Deleter> p_heap = std::unique_ptr<RootBlock, Deleter>(new (ptr) RootBlock("RootHeap"));
    p_heap->mBackwardTag.mAllSize = 0;
    p_heap->mSize = static_cast<std::uint32_t>(size);

    // フリーの大きなコンテナを作っておく
    constexpr const std::size_t cManagementSize = sizeof(RootBlock);
    void* containerPtr = reinterpret_cast<std::uint8_t*>(ptr) + cManagementSize;
    Container* p_container = new (containerPtr) Container(static_cast<std::uint32_t>(size - cManagementSize - Container::cAllTagSize - Container::cForwardTagSize));

    // 末端がわかるようにタグを入れておく
    std::uint8_t* p_final_tag = reinterpret_cast<std::uint8_t*>(p_container) + p_container->getAllSize();
    Container::BackwardTag* p_backward_tag = reinterpret_cast<Container::BackwardTag*>(p_final_tag);
    p_backward_tag->mAllSize = 0;

    return p_heap;
}

// ----------------------------------------------------------------------------

void* RootBlock::allocImpl_(std::size_t size)
{
    // フリーのコンテナを分割して利用する
    constexpr const std::size_t cManagementSize = sizeof(RootBlock);
    Container* p_container = reinterpret_cast<Container*>(reinterpret_cast<std::uint8_t*>(this) + cManagementSize);

    // 線形検索して、空いてたら利用
    while (true)
    {
        if (p_container->isFree() && p_container->getSize() >= size)
        {
            // サイズに余裕があれば分割して利用する
            Container* p_target = p_container->split(static_cast<std::uint32_t>(size));
            if (!p_target)
            {
                p_target = p_container;
            }
            p_target->setFree(false);
            return p_target->getData();
        }
        else
        {
            p_container = p_container->getNext();
            // ヒープ領域超えたらアウト
            std::uint8_t* p_limit_ptr = reinterpret_cast<std::uint8_t*>(this) + mSize;
            if (reinterpret_cast<std::uintptr_t>(p_container) >= reinterpret_cast<std::uintptr_t>(p_limit_ptr))
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
