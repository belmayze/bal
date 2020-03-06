/*!
 * @file   balLog_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <cstdio>

// ----------------------------------------------------------------------------

namespace bal {

template <typename... Formats>
void Log::Print(Formats... args)
{
    constexpr int cMaxLength = 1024;
    char str[cMaxLength];
#if BAL_PLATFORM_IS_WIN
    sprintf_s(str, args...);
#else
    sprintf(str, args...);
#endif
    Print_(str);
}

}