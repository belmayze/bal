/*!
 * @file   balHeapBlockOS.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <heap/balHeapBlockBase.h>
#include <memory/balUniquePtr.h>

// ----------------------------------------------------------------------------
namespace bal::heap {

class BlockOS : public BlockBase
{
public:
    /*!
     * ヒープを作成する
     */
    static std::unique_ptr<BlockOS> Create(const StringPtr& name);

private:
    /*!
     * コンストラクター
     */
    BlockOS(const StringPtr& name) :BlockBase(name) {}

    /*!
     * メモリー確保
     */
    virtual void* allocImpl_(size_t size) override;
};

}
