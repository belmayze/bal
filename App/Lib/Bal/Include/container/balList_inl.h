/*!
 * @file   balList_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balList.h>

namespace bal {

// ----------------------------------------------------------------------------
template <typename T>
void List<T>::initialize(size_t max_size)
{
    mpNodeBuffer = bal::make_unique<Node[]>(nullptr, max_size);
    mpFreeList = bal::make_unique<Node* []>(nullptr, max_size);
    for (size_t i_node = 0; i_node < max_size; ++i_node)
    {
        mpNodeBuffer[i_node].alloc();
        mpFreeList[i_node] = &mpNodeBuffer[i_node];
    }
    mMaxSize      = max_size;
    mFreeListSize = max_size;

    mDummyNode.setDummy();
}

// ----------------------------------------------------------------------------
template <typename T>
void List<T>::push_front(const T& t)
{
    BAL_ASSERT(mFreeListSize > 0);

    Node* p_node = mpFreeList[--mFreeListSize];
    p_node->construct(t);

    // 先頭に追加
    mDummyNode.linkNext(p_node);
}
template <typename T>
void List<T>::push_front(T&& t)
{
    BAL_ASSERT(mFreeListSize > 0);

    Node* p_node = mpFreeList[--mFreeListSize];
    p_node->construct(std::move(t));

    // 先頭に追加
    mDummyNode.linkNext(p_node);
}

// ----------------------------------------------------------------------------
template <typename T>
void List<T>::push_back(const T& t)
{
    BAL_ASSERT(mFreeListSize > 0);

    Node* p_node = mpFreeList[--mFreeListSize];
    p_node->construct(t);

    // 末尾に追加
    mDummyNode.linkPrev(p_node);
}
template <typename T>
void List<T>::push_back(T&& t)
{
    BAL_ASSERT(mFreeListSize > 0);

    Node* p_node = mpFreeList[--mFreeListSize];
    p_node->construct(std::move(t));

    // 末尾に追加
    mDummyNode.linkPrev(p_node);
}
// ----------------------------------------------------------------------------
template <typename T>
void List<T>::pop_front()
{
    BAL_ASSERT(!empty());

    Node* p_node = mDummyNode.getNext();
    p_node->unlink();
    p_node->destruct();
    mpFreeList[mFreeListSize++] = p_node;
}
// ----------------------------------------------------------------------------
template <typename T>
void List<T>::pop_back()
{
    BAL_ASSERT(!empty());

    Node* p_node = mDummyNode.getPrev();
    p_node->unlink();
    p_node->destruct();
    mpFreeList[mFreeListSize++] = p_node;
}

// ----------------------------------------------------------------------------
template <typename T>
void List<T>::clear()
{
    while (!empty())
    {
        pop_back();
    }
}
// ----------------------------------------------------------------------------
template <typename T>
T& List<T>::front()
{
    BAL_ASSERT(!empty());
    return *begin();
}
template <typename T>
const T& List<T>::front() const
{
    BAL_ASSERT(!empty());
    return *begin();
}

// ----------------------------------------------------------------------------
template <typename T>
T& List<T>::back()
{
    BAL_ASSERT(!empty());
    iterator it = end();
    return *(--it);
}
template <typename T>
const T& List<T>::back() const
{
    BAL_ASSERT(!empty());
    const_iterator it = end();
    return *(--it);
}
// ----------------------------------------------------------------------------

}
