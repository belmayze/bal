/*!
 * @file   balStringPtr.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * ポインターのみ保持している文字列クラス
 */
class StringPtr
{
public:
    /*! ポインターから初期化 */
    constexpr StringPtr(const char* p) :mpStr(p) { BAL_ASSERT(mpStr != nullptr); calcSize_(); }

    /*!
     * 文字列ポインター取得
     */
    constexpr const char* c_str() const { return mpStr; }

    /*!
     * 文字列の長さを取得
     */
    constexpr std::size_t size() const { return mSize; }

    /*!
     * 文字列の長さを取得
     */
    constexpr std::size_t length() const { return mSize; }

    /*!
     * 空文字かチェック
     */
    constexpr bool empty() const { return *mpStr == '\0'; }

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
