/*!
 * @file   balString.h
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
 * 文字列
 */
template <std::size_t N>
class String
{
    BAL_STATIC_ASSERT(N > 0, "String<N>: N > 0");
public:
    /*! ポインターからコピー */
    String(const char* str) { assign(str); }

    /*!
     * 文字列ポインター取得
     */
    const char* c_str() const { return mStr; }

    /*!
     * 文字列を代入する
     */
    String<N>& assign(const StringPtr& str);

private:
    char        mStr[N];
    std::size_t mSize = 0;
};

}

#include <container/balString_inl.h>
