/*!
 * @file   balHeapRoot.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>
#include <heap/balHeapBase.h>

namespace bal { class Framework; }

// ----------------------------------------------------------------------------
namespace bal {

class HeapRoot : public HeapBase
{
    friend Framework; // ルートヒープが唯一作れるクラス

public:
    struct Deleter
    {
        void operator()(void* ptr) const;
    };

private:
    /*!
     * ルートヒープの作成
     * @note フレームワークからのみ作成できます
     */
    static UniquePtr<HeapRoot, Deleter> Create(std::size_t size);

private:
    /*!
     * コンストラクター
     */
    HeapRoot(const StringPtr& name) :HeapBase(name) {}

    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(std::size_t size);
};

}
