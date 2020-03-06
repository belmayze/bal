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
    printf(str);

#if BAL_PLATFORM_IS_WIN
    {
#   if defined(UNICODE)
        UniquePtr<TCHAR[]> tchar;
        int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
        tchar = std::make_unique<TCHAR[]>(len);
        MultiByteToWideChar(CP_ACP, 0, str, -1, tchar.get(), len);
        OutputDebugString(tchar.get());
#   else
        OutputDebugString(str);
#   endif // UNICODE
    }
#endif
}

}
