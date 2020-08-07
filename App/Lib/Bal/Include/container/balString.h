/*!
 * @file   balString.h
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
 * 文字列
 */
template <size_t N>
class String : public StringBase
{
    BAL_STATIC_ASSERT(N > 0, "String<N>: N > 0");
public:
    /*! ポインターからコピー */
    String(const char* str) { assign(str); }

    /*! デストラクター */
    virtual ~String() {}

    /*!
     * 文字列ポインター取得
     */
    virtual const char* c_str() const override { return mStr; }

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
    virtual bool empty() const override { return mStr[0] == '\0'; }

    /*!
     * 文字列を代入する
     * @param[in] str 代入する文字列
     */
    String<N>& assign(const StringBase& str);

private:
    char   mStr[N];
    size_t mSize = 0;
};

}

#include <container/balString_inl.h>
