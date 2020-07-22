/*!
 * @file   balHeapBlockBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBlockBase.h>
#include <heap/balHeapManager.h>
#include <math/balMath.h>

namespace bal::heap {

void* BlockBase::alloc(size_t size, size_t alignment)
{
    size_t alignmented_size = Math::Ceil(size, alignment);
    return allocImpl_(alignmented_size);
}

}

// ヒープ指定なしで確保
void* operator new(size_t size)
{
    bal::heap::BlockBase* p_current_heap = bal::heap::Manager::GetInstance().getCurrentHeap();
    return p_current_heap->alloc(size);
}
void* operator new(size_t size, bal::heap::BlockBase* p_heap)
{
    return p_heap->alloc(size);
}
void* operator new[](size_t size)
{
    bal::heap::BlockBase* p_current_heap = bal::heap::Manager::GetInstance().getCurrentHeap();
    return p_current_heap->alloc(size);
}
void* operator new[](size_t size, bal::heap::BlockBase* p_heap)
{
    return p_heap->alloc(size);
}
// 破棄
void operator delete(void* ptr)
{
    // タグチェック
    bal::heap::Container* p_container = reinterpret_cast<bal::heap::Container*>(reinterpret_cast<uint8_t*>(ptr) - bal::heap::Container::cForwardTagSize);
    p_container->setFree(true);
    p_container->setFill(0xBB);
    // 前後がフリーの場合は統合する
    p_container->merge();
}
void operator delete(void* ptr, bal::heap::BlockBase* p_heap)
{
    // タグチェック
    bal::heap::Container* p_container = reinterpret_cast<bal::heap::Container*>(reinterpret_cast<uint8_t*>(ptr) - bal::heap::Container::cForwardTagSize);
    p_container->setFree(true);
    p_container->setFill(0xBB);
    // 前後がフリーの場合は統合する
    p_container->merge();
}
void operator delete[](void* ptr)
{
    // タグチェック
    bal::heap::Container* p_container = reinterpret_cast<bal::heap::Container*>(reinterpret_cast<uint8_t*>(ptr) - bal::heap::Container::cForwardTagSize);
    p_container->setFree(true);
    p_container->setFill(0xBB);
    // 前後がフリーの場合は統合する
    p_container->merge();
}
void operator delete[](void* ptr, bal::heap::BlockBase* p_heap)
{
    // タグチェック
    bal::heap::Container* p_container = reinterpret_cast<bal::heap::Container*>(reinterpret_cast<uint8_t*>(ptr) - bal::heap::Container::cForwardTagSize);
    p_container->setFree(true);
    p_container->setFill(0xBB);
    // 前後がフリーの場合は統合する
    p_container->merge();
}
