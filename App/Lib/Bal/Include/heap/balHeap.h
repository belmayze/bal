/*!
 * @file   balHeap.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <heap/balHeapBase.h>

// ----------------------------------------------------------------------------
namespace bal {

class Heap : public HeapBase
{
public:
    /*!
     * ヒープを作成します
     * @param[in] size     サイズ
     * @param[in] name     名前
     * @param[in] p_parent 親ヒープ
     */
    static std::unique_ptr<Heap> Create(std::size_t size, const StringPtr& name, HeapBase* p_parent);

private:
    std::unique_ptr<std::uint8_t[]> mpRootPtr;

private:
    /*!
     * コンストラクター
     */
    Heap(const StringPtr& name) :HeapBase(name) {}

    /*!
     * 確保内部関数
     * @param[in] size アライメント済みのサイズ
     */
    virtual void* allocImpl_(std::size_t size);
};

}
