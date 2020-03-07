/*!
 * @file   balHeapBase.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * ヒープ基底クラス
 */
class HeapBase
{
public:
    /*! デストラクター */
    virtual ~HeapBase() {}

    /*!
     * メモリー確保
     * @param[in] size      サイズ
     * @param[in] alignment アライメント
     */
    void* alloc(std::size_t size, std::size_t alignment = 4);

protected:
    const StringPtr mName;

protected:
    // コンストラクターで作ることはできません
    HeapBase() = delete;
    HeapBase(const StringPtr& name): mName(name) {}
    HeapBase(const HeapBase&) = delete;

private:
    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(std::size_t size) = 0;
};

}
