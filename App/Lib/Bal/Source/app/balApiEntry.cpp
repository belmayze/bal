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
#include <app/balApplicationBase.h>

namespace bal
{

// ----------------------------------------------------------------------------

void ApiEntry::setupOptions(int argc, char* argv[])
{
    // @TODO: オプション解析
}

// ----------------------------------------------------------------------------

int ApiEntry::boot(ApplicationBase* p_application)
{
    // アプリケーションチェック
    if (!p_application) { return -1; }

    // 起動モードチェック
    p_application->setupBoot(*this, &mBootArg);

    if (mBootArg.mBootMode == BootMode::Console)
    {
        // コンソールモードであれば、コンソールウィンドウを開いておく
        AllocConsole();
        int console_handle = _open_osfhandle(reinterpret_cast<intptr_t>(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
        FILE* fp = _fdopen(console_handle, "w");
        freopen_s(&fp, "CONOUT$", "w", stdout);
    }

    return p_application->main(*this);
}

// ----------------------------------------------------------------------------

}

/*!
 * Windows の API エントリーポイント
 */
int APIENTRY WinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPSTR     p_cmd_line,
    int       cmd_show
)
{
    // オプションの解析
    bal::ApiEntry api_entry;
    api_entry.setupOptions(__argc, __argv);

    // ブート
    return api_entry.boot(bal::gpApplication);
}
