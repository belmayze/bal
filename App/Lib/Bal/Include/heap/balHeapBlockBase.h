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
#include <heap/balHeapContainer.h>

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
    const StringPtr        mName;
    std::uint32_t          mSize;
    Container::BackwardTag mBackwardTag;

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

void* operator new(std::size_t size);
void* operator new(std::size_t size, bal::heap::BlockBase* p_heap);

void operator delete(void* ptr);
void operator delete(void* ptr, bal::heap::BlockBase* p_heap);
