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
namespace bal::heap { class BlockBase; }

// ----------------------------------------------------------------------------
namespace bal::heap {

class Manager : public Singleton<Manager>
{
public:
    /*! 
     * カレントヒープを設定します
     * @param[in] p_heap ヒープ
     */
    void setCurrentHeap(BlockBase* p_heap) { mpCurrentHeap = p_heap; }

    /*!
     * ヒープを取得する
     */
    BlockBase* getCurrentHeap() { return mpCurrentHeap; }

private:
    BlockBase* mpCurrentHeap = nullptr;
};

}
