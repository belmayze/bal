/*!
 * @file   balHeapRootBlock.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>
#include <heap/balHeapBlockBase.h>

namespace bal { class Framework; }

// ----------------------------------------------------------------------------
namespace bal::heap {

class RootBlock : public BlockBase
{
    friend Framework; // ルートヒープが唯一作れるクラス

public:
    // ヒープ破棄関数
    struct Deleter
    {
        void operator()(void* ptr) const;
    };

private:
    /*!
     * ルートヒープの作成
     * @note フレームワークからのみ作成できます
     */
    static std::unique_ptr<RootBlock, Deleter> Create(size_t size);

private:
    /*!
     * コンストラクター
     */
    RootBlock(const StringPtr& name) :BlockBase(name) {}

    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(size_t size);
};

}
