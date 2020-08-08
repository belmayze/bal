/*!
 * @file   balStringBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringBase.h>
#include <memory/balUniquePtr.h>

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * 文字列（内部でバッファを持っています）
 */
class StringBuffer : public StringBase
{
public:
    /*! デフォルトコンストラクター */
    StringBuffer() {}

    /*! ポインターからコピー */
    StringBuffer(const char* str);

    /*! 文字列を指定された文字だけコピーする */
    StringBuffer(const char* str, size_t length);

    /*! デストラクター */
    virtual ~StringBuffer() {}

    /*!
     * 文字列ポインター取得
     */
    virtual const char* c_str() const override { return reinterpret_cast<const char*>(mpStr.get()); }

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
    virtual bool empty() const override { return (!mpStr || mpStr[0] == '\0'); }

public:
    //! コピー
    StringBuffer& operator = (StringBuffer&& rhs);

private:
    std::unique_ptr<uint8_t[]> mpStr;
    size_t                     mSize = 0;

private:
    /*! 文字列の長さを計算する */
    constexpr void calcSize_(const char* str)
    {
        const char* p = str;
        for (int i = 0; i < 1024 * 1024; ++i)
        {
            if (*p == '\0') { mSize = i; return; }
            ++p;
        }
    }
};

}
