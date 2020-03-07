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

UniquePtr<RootBlock, RootBlock::Deleter> RootBlock::Create(std::size_t size)
{
    // ルートヒープは OS から確保
    void* ptr = malloc(size);
    UniquePtr<RootBlock, Deleter> p_heap = UniquePtr<RootBlock, Deleter>(new (ptr) RootBlock("RootHeap"));

    // フリーの大きなコンテナを作っておく
    std::size_t managementSize = sizeof(RootBlock);
    void* containerPtr = reinterpret_cast<std::uint8_t*>(ptr) + managementSize;
    Container* p_container = new (containerPtr) Container(static_cast<std::uint32_t>(size - managementSize));

    return p_heap;
}

// ----------------------------------------------------------------------------

void* RootBlock::allocImpl_(std::size_t size)
{
    return nullptr;
}

// ----------------------------------------------------------------------------

}
