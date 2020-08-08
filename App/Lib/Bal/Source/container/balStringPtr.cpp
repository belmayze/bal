﻿/*!
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

std::pair<std::unique_ptr<StringBuffer[]>, uint32_t> StringPtr::getLines() const
{
    uint32_t num_split = 0;
    std::unique_ptr<std::pair<size_t, size_t>[]> positions = make_unique<std::pair<size_t, size_t>[]>(nullptr, mSize);

    // 区切り文字の数をチェックする
    int64_t npos = -1;
    for (size_t i_str = 0; i_str < mSize; ++i_str)
    {
        if (i_str == (mSize - 1))
        {
            if (i_str != static_cast<size_t>(npos + 1))
            {
                // 最後
                positions[num_split].first  = (npos + 1);
                positions[num_split].second = i_str;
                num_split++;
            }
        }
        else if (mpStr[i_str] == '\r' || mpStr[i_str] == '\n')
        {
            if (i_str == static_cast<size_t>(npos + 1))
            {
                // 連続して同じ区切り文字であれば除外
                npos = i_str;
            }
            else
            {
                // 開始と終了の記録
                positions[num_split].first  = (npos + 1);
                positions[num_split].second = i_str;
                num_split++;
                npos = i_str;
            }
        }
    }

    // 個数分初期化しておく
    std::unique_ptr<StringBuffer[]> buffer = make_unique<StringBuffer[]>(nullptr, num_split);
    for (uint32_t i_split = 0; i_split < num_split; ++i_split)
    {
        buffer[i_split] = StringBuffer(mpStr + positions[i_split].first, positions[i_split].second - positions[i_split].first);
    }

    return {std::move(buffer), num_split};
}

// ----------------------------------------------------------------------------

std::unique_ptr<StringBuffer[]> StringPtr::split(char word) const
{
    return nullptr;
}

// ----------------------------------------------------------------------------
}
