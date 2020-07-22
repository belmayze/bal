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
    void* alloc(size_t size, size_t alignment = 4);

protected:
    const StringPtr            mName;
    std::unique_ptr<uint8_t[]> mpRootPtr;
    uint32_t                   mSize = 0;
    Container::BackwardTag     mBackwardTag;

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
    virtual void* allocImpl_(size_t size) = 0;
};

}

void* operator new(size_t size);
void* operator new(size_t size, bal::heap::BlockBase* p_heap);

void* operator new[](size_t size);
void* operator new[](size_t size, bal::heap::BlockBase* p_heap);

void operator delete(void* ptr);
void operator delete(void* ptr, bal::heap::BlockBase* p_heap);

void operator delete[](void* ptr);
void operator delete[](void* ptr, bal::heap::BlockBase* p_heap);
