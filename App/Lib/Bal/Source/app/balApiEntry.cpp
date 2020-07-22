/*!
 * @file   balApiEntry.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
// bal
#include <app/balApiEntry.h>

namespace bal
{

}

/*!
 * Windows の API エントリーポイント
 */
int APIENTRY WinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPSTR     argv,
    int       argc
)
{
    AllocConsole();
    int console_handle = _open_osfhandle(reinterpret_cast<intptr_t>(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
    FILE* fp = _fdopen(console_handle, "w");
    freopen_s(&fp, "CONOUT$", "w", stdout);

    int exit_code = balMain();

    //_close(console_handle);
    //FreeConsole();

    return exit_code;
}
