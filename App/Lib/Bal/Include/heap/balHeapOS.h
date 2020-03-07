/*!
 * @file   balHeapOS.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <heap/balHeapBase.h>
#include <memory/balUniquePtr.h>

// ----------------------------------------------------------------------------
namespace bal {

class HeapOS : public HeapBase
{
public:
    /*!
     * ヒープを作成する
     */
    static UniquePtr<HeapOS> Create(const StringPtr& name);

private:
    /*!
     * コンストラクター
     */
    HeapOS(const StringPtr& name) :HeapBase(name) {}

    /*!
     * メモリー確保
     */
    virtual void* allocImpl_(std::size_t size) override;
};

}
