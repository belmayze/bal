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
    calcSize_(str);
    StringBuffer(str, mSize);
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

StringBuffer& StringBuffer::operator = (StringBuffer&& rhs)
{
    mSize = rhs.mSize;
    mpStr = std::move(rhs.mpStr);
    return *this;
}

// ----------------------------------------------------------------------------

}