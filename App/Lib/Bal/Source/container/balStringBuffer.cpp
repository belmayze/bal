/*!
 * @file   balString.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balStringBuffer.h>

namespace bal
{

// ----------------------------------------------------------------------------

StringBuffer::StringBuffer(const char* str)
{
    size_t length = calcSize_(str);
    StringBuffer(str, length);
}

// ----------------------------------------------------------------------------

StringBuffer::StringBuffer(const char* str, size_t length)
{
    mSize = length;
    mpStr = make_unique<uint8_t[]>(nullptr, mSize + 1);
    std::memcpy(mpStr.get(), str, mSize);
    mpStr[mSize] = '\0';
}

// ----------------------------------------------------------------------------

bool StringBuffer::isEquals(const StringBase& str) const
{
    // 文字列の長さが異なれば絶対違う
    if (mSize != str.size()) { return false; }

    // 同じ文字列なら一文字ずつチェックする
    return std::strcmp(reinterpret_cast<const char*>(mpStr.get()), str.c_str()) == 0;
}

// ----------------------------------------------------------------------------

std::pair<uint32_t, std::unique_ptr<StringBuffer[]>> StringBuffer::getLines() const
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
                positions[num_split].first = (npos + 1);
                positions[num_split].second = i_str + 1;
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
                positions[num_split].first = (npos + 1);
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
        buffer[i_split] = StringBuffer(reinterpret_cast<char*>(mpStr.get()) + positions[i_split].first, positions[i_split].second - positions[i_split].first);
    }

    return { num_split, std::move(buffer) };
}

// ----------------------------------------------------------------------------

std::pair<uint32_t, std::unique_ptr<StringBuffer[]>> StringBuffer::getArgv() const
{
    uint32_t num_split = 0;
    std::unique_ptr<std::pair<size_t, size_t>[]> positions = make_unique<std::pair<size_t, size_t>[]>(nullptr, mSize);

    // 区切り文字の数をチェックする
    int64_t npos = -1;
    bool    is_escape = false;
    for (size_t i_str = 0; i_str < mSize; ++i_str)
    {
        if (i_str == (mSize - 1))
        {
            if (i_str != static_cast<size_t>(npos + 1))
            {
                // 最後
                positions[num_split].first = (npos + 1);
                positions[num_split].second = i_str + 1;
                num_split++;
            }
        }
        else if (!is_escape && mpStr[i_str] == ' ')
        {
            if (i_str == static_cast<size_t>(npos + 1))
            {
                // 連続して同じ区切り文字であれば除外
                npos = i_str;
            }
            else
            {
                // 開始と終了の記録
                positions[num_split].first = (npos + 1);
                positions[num_split].second = i_str;
                num_split++;
                npos = i_str;
            }
        }
        else if (mpStr[i_str] == '"')
        {
            // エスケープチェック
            is_escape = !is_escape;
        }
    }

    // 個数分初期化しておく
    std::unique_ptr<StringBuffer[]> buffer = make_unique<StringBuffer[]>(nullptr, num_split);
    for (uint32_t i_split = 0; i_split < num_split; ++i_split)
    {
        buffer[i_split] = StringBuffer(reinterpret_cast<char*>(mpStr.get()) + positions[i_split].first, positions[i_split].second - positions[i_split].first);
    }

    return { num_split, std::move(buffer) };
}

// ----------------------------------------------------------------------------

StringBuffer& StringBuffer::operator = (StringBuffer&& rhs)
{
    mSize = rhs.mSize;
    mpStr = std::move(rhs.mpStr);
    return *this;
}

// ----------------------------------------------------------------------------

}