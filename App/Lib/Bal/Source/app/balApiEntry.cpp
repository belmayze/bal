/*!
 * @file   balApiEntry.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
// bal
#include <app/balApiEntry.h>
#include <app/balApplicationBase.h>
#include <app/balOptionParser.h>
#include <framework/balFramework.h>
#include <heap/balHeapManager.h>

namespace bal
{

// ----------------------------------------------------------------------------

ApiEntry::~ApiEntry()
{
    destroy_();
}

// ----------------------------------------------------------------------------

int ApiEntry::boot(ApplicationBase* p_application, bool enable_window, int argc, const char* argv[])
{
    // アプリケーションチェック
    if (!p_application) { return -1; }

    // 起動モードチェック
    p_application->setupBoot(&mBootArg);

    // ルートヒープを確保
    mpRootHeap = heap::RootBlock::Create(mBootArg.mRootHeapSize);
    heap::Manager::GetInstance(mpRootHeap.get()).setCurrentHeap(mpRootHeap.get());

    // オプション解析（先頭には実行ファイルパスが入っているので除く）
    mpOptionParser = make_unique<OptionParser>(nullptr);
    mpOptionParser->parse(argc - 1, argv + 1);

    if (enable_window && mBootArg.mBootMode == BootMode::Console)
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

uint32_t ApiEntry::getNumOption() const
{
    return mpOptionParser->getNumOption();
}

// ----------------------------------------------------------------------------

const std::pair<StringPtr, StringPtr>* ApiEntry::getOptions() const
{
    return mpOptionParser->getOptions();
}

// ----------------------------------------------------------------------------

void ApiEntry::destroy_()
{
    // ルートヒープよりも先に開放すべきものがあればここに記述する
    mpOptionParser.reset();
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
    // ブート
    int exit_code = 0;
    {
        bal::ApiEntry api_entry;
        int          argc = __argc;
        const char** argv = const_cast<const char**>(__argv);
        exit_code = api_entry.boot(bal::gpApplication, true, argc, argv);
    }
    return exit_code;
}

int main(int argc, const char** argv)
{
    // ブート
    int exit_code = 0;
    {
        bal::ApiEntry api_entry;
        exit_code = api_entry.boot(bal::gpApplication, false, argc, argv);
    }
    return exit_code;
}
