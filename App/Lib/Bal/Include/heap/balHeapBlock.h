/*!
 * @file   balHeapBlock.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <heap/balHeapBlockBase.h>

// ----------------------------------------------------------------------------
namespace bal::heap {

class Block : public BlockBase
{
public:
    /*!
     * ヒープを作成します
     * @param[in] size     サイズ
     * @param[in] name     名前
     * @param[in] p_parent 親ヒープ
     */
    static std::unique_ptr<Block> Create(size_t size, const StringPtr& name, BlockBase* p_parent);

private:
    /*!
     * コンストラクター
     */
    Block(const StringPtr& name) :BlockBase(name) {}

    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(size_t size);
};

}
