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
void Log::Print(EType type, Formats... args)
{
#   if BAL_BUILD_UNDER_DEVELOPMENT
    constexpr int cMaxLength = 1024;
    char str[cMaxLength];
    snprintf(str, cMaxLength, args...);
    Print_(str);
#   else
    if (type == EType::Error)
    {
        constexpr int cMaxLength = 1024;
        char str[cMaxLength];
        snprintf(str, cMaxLength, args...);
        Print_(str);
    }
#   endif // BAL_BUILD_UNDER_DEVELOPMENT
}

}