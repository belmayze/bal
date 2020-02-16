/*!
 * @file   balHeapManager.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <memory/balSingleton.h>

// 前方宣言
namespace bal { class HeapBase; }

// ----------------------------------------------------------------------------
namespace bal {

class HeapManager : public Singleton<HeapManager>
{
public:
    /*! 
     * カレントヒープを設定します
     * @param[in] p_heap ヒープ
     */
    void setCurrentHeap(HeapBase* p_heap) { mpCurrentHeap = p_heap; }

private:
    HeapBase* mpCurrentHeap = nullptr;
};

}
