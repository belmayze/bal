/*!
 * @file   balTreeMap_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balTreeMap.h>

namespace bal {

// ----------------------------------------------------------------------------
template <typename T>
void TreeMap<T>::initialize(size_t max_size)
{
    mpNodeBuffer = bal::make_unique<Node[]>(nullptr, max_size);
    mpFreeList  = bal::make_unique<Node* []>(nullptr, max_size);
    for (size_t i_node = 0; i_node < max_size; ++i_node)
    {
        mpNodeBuffer[i_node].alloc();
        mpFreeList[i_node] = &mpNodeBuffer[i_node];
    }
    mMaxSize = max_size;
    mFreeListSize = max_size;
}
// ----------------------------------------------------------------------------
template <typename T>
void TreeMap<T>::remove(Node* p_node)
{
    BAL_ASSERT(p_node != nullptr);

    // 子がいればすべて削除する
    Node* p_child = p_node->getChild();
    while (p_child != nullptr)
    {
        remove(p_child);
        p_child = p_node->getChild();
    }

    // 子がいなくなったら自分自身を削除する
    p_node->unlink();
    p_node->destruct();
    mpFreeList[mFreeListSize++] = p_node;
}
// ----------------------------------------------------------------------------
template <typename T>
void TreeMap<T>::clear()
{
    // ダミーノードに刺さっている子すべて削除していく
    Node* p_child = mDummyNode.getChild();
    while (p_child != nullptr)
    {
        remove(p_child);
        p_child = mDummyNode.getChild();
    }
}
// ----------------------------------------------------------------------------

}
