/*!
 * @file   balHeapBlockBase.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>

namespace bal::heap { class Container; }

// ----------------------------------------------------------------------------
namespace bal::heap {

/*!
 * ヒープ基底クラス
 */
class BlockBase
{
public:
    /*! デストラクター */
    virtual ~BlockBase() {}

    /*!
     * メモリー確保
     * @param[in] size      サイズ
     * @param[in] alignment アライメント
     */
    void* alloc(std::size_t size, std::size_t alignment = 4);

protected:
    const StringPtr mName;
    Container*      mpTopContainer;

protected:
    // コンストラクターで作ることはできません
    BlockBase() = delete;
    BlockBase(const StringPtr& name): mName(name) {}
    BlockBase(const BlockBase&) = delete;

private:
    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(std::size_t size) = 0;
};

}
