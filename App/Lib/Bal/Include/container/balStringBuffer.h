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

namespace bal { class StringPtr; }

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
    virtual bool isEmpty() const override { return (!mpStr || mpStr[0] == '\0'); }

public:
    /*!
     * 一致しているかをチェック
     * @param[in] str 対象文字列
     */
    bool isEquals(const StringBase& str) const;

    /*!
     * 文字列を行で分割します
     */
    std::pair<uint32_t, std::unique_ptr<StringBuffer[]>> getLines() const;

    /*!
     * 文字列を argc argv 形式に分割します
     */
    std::pair<uint32_t, std::unique_ptr<StringBuffer[]>> getArgv() const;

public:
    //! コピー
    StringBuffer& operator = (StringBuffer&& rhs);

private:
    std::unique_ptr<uint8_t[]> mpStr;
    size_t                     mSize = 0;

private:
    /*! 文字列の長さを計算する */
    constexpr size_t calcSize_(const char* str)
    {
        const char* p = str;
        constexpr size_t cMaxLength = 1024 * 1024;
        for (size_t i = 0; i < cMaxLength; ++i)
        {
            if (*p == '\0') { return i; }
            ++p;
        }
        return cMaxLength;
    }
};

}
