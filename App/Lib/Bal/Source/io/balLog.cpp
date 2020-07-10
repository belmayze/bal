/*!
 * @file   balLog.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <io/balLog.h>
#include <memory/balUniquePtr.h>

namespace bal {

void Log::Print_(char* str)
{
    printf_s(str);

#if BAL_PLATFORM_IS_WIN
    {
#   if defined(UNICODE)
        TCHAR* tchar = nullptr;
        int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
        tchar = reinterpret_cast<TCHAR*>(malloc(len * sizeof(TCHAR)));
        MultiByteToWideChar(CP_ACP, 0, str, -1, tchar, len);
        OutputDebugString(tchar);
        free(tchar);
#   else
        OutputDebugString(str);
#   endif // UNICODE
    }
#endif
}

}
