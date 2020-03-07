/*!
 * @file   balStringPtr.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringBase.h>

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * ポインターのみ保持している文字列クラス
 * 使用メモリーは少ないですが、内部文字列は変更できません
 */
class StringPtr : public StringBase
{
public:
    /*! 空文字で初期化 */
    constexpr StringPtr() :StringPtr("") { }

    /*! ポインターから初期化 */
    constexpr StringPtr(const char* p) :mpStr(p) { BAL_ASSERT(mpStr != nullptr); calcSize_(); }

    /*! デストラクター */
    virtual ~StringPtr() {}

    /*!
     * 文字列ポインター取得
     */
    virtual const char* c_str() const override { return mpStr; }

    /*!
     * 文字列の長さを取得
     */
    virtual std::size_t size() const override { return mSize; }

    /*!
     * 文字列の長さを取得
     */
    virtual std::size_t length() const override { return mSize; }

    /*!
     * 空文字かチェック
     */
    virtual bool empty() const override { return *mpStr == '\0'; }

private:
    const char* mpStr = "";
    std::size_t mSize = 0;

private:
    /*! 文字列の長さを計算する */
    constexpr void calcSize_()
    {
        const char* p = mpStr;
        for (int i = 0; i < 1024 * 1024; ++i)
        {
            if (*p == '\0') { mSize = i; return; }
            ++p;
        }
    }
};

}
