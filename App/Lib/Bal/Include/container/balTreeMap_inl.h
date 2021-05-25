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

    mDummyNode.setDummy();
}
// ----------------------------------------------------------------------------

}
