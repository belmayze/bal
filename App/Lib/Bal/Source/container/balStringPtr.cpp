/*!
 * @file   balStringPtr.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balStringPtr.h>

namespace bal
{

// ----------------------------------------------------------------------------

bool StringPtr::isEquals(const StringPtr& str) const
{
    // 文字列の長さが異なれば絶対違う
    if (mSize != str.size()) { return false; }

    // 同じ文字列なら一文字ずつチェックする
    return std::strcmp(mpStr, str.c_str()) == 0;
}

// ----------------------------------------------------------------------------

std::unique_ptr<StringBuffer[]> StringPtr::split(const char* p) const
{
    // 区切り文字の数をチェックする
    uint32_t num_split = 0;
    size_t   npos      = 0;
    for (size_t i_str = 0; i_str < mSize; ++i_str)
    {
        // @TODO
    }

    return nullptr;
}

// ----------------------------------------------------------------------------
}
