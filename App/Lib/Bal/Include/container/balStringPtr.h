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
#include <container/balStringBuffer.h>

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
    constexpr StringPtr(const char* p, int32_t length = -1) : mpStr(p) { BAL_ASSERT(mpStr != nullptr); if (length < 0) { calcSize_(); } else { mSize = static_cast<size_t>(length); } }

    /*! デストラクター */
    virtual ~StringPtr() {}

    /*!
     * 文字列ポインター取得
     */
    virtual const char* c_str() const override { return mpStr; }

    /*!
     * 文字列の長さを取得
     */
    virtual size_t size() const override { return mSize; }

    /*!
     * 文字列の長さを取得
     */
    virtual size_t length() const override { return mSize; }

    /*!
     * 空文字かチェック
     */
    virtual bool empty() const override { return *mpStr == '\0'; }

public:
    /*!
     * 一致しているかをチェック
     * @param[in] str 対象文字列
     */
    bool isEquals(const StringPtr& str) const;

    /*!
     * 文字列を行で分割します
     */
    std::pair<std::unique_ptr<StringBuffer[]>, uint32_t> getLines() const;

    /*!
     * 特定の文字で分割します
     */
    std::unique_ptr<StringBuffer[]> split(char word) const;

private:
    const char* mpStr = "";
    size_t      mSize = 0;

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
