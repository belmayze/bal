/*!
 * @file   balString_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <memory/balMemUtil.h>

namespace bal {

// ----------------------------------------------------------------------------

template <std::size_t N>
String<N>& String<N>::assign(const StringBase& str)
{
    std::size_t size = str.size();
    if (size >= N)
    {
        BAL_ASSERT(size < N);
        MemUtil::Copy(mStr, str.c_str(), N - 1);
        mStr[N - 1] = '\0';
        mSize = N - 1;
    }
    else
    {
        MemUtil::Copy(mStr, str.c_str(), size + 1);
        mSize = size;
    }
    return *this;
}

// ----------------------------------------------------------------------------

}
