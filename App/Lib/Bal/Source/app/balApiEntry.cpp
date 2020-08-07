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

int ApiEntry::boot(ApplicationBase* p_application, bool enable_window, int argc, char* argv[])
{
    // アプリケーションチェック
    if (!p_application) { return -1; }

    // 起動モードチェック
    p_application->setupBoot(&mBootArg);

    // ルートヒープを確保
    mpRootHeap = heap::RootBlock::Create(mBootArg.mRootHeapSize);
    heap::Manager::GetInstance(mpRootHeap.get()).setCurrentHeap(mpRootHeap.get());

    // オプション解析
    setupOptions_(argc, argv);

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

void ApiEntry::setupOptions_(int argc, char* argv[])
{
    // argv[0] には実行 exe パスが入るので除外
    argv++;

    // 解析
    uint32_t option_index = 0;
    std::unique_ptr<std::unique_ptr<std::pair<StringPtr, StringPtr>>[]> arg_list = make_unique<std::unique_ptr<std::pair<StringPtr, StringPtr>>[]>(nullptr, argc);

    char* p_token = nullptr;
    for (int i_arg = 0; i_arg < (argc - 1); ++i_arg)
    {
        if (*argv[i_arg] == '-')
        {
            // - から始まればオプションキー
            if (p_token)
            {
                // token があれば、無指定でオプション追加
                arg_list[option_index] = make_unique<std::pair<StringPtr, StringPtr>>(nullptr, p_token, StringPtr());
                option_index++;
            }

            p_token = argv[i_arg];
        }
        else
        {
            // それ以外はオプションバリュー
            arg_list[option_index] = make_unique<std::pair<StringPtr, StringPtr>>(nullptr, p_token ? p_token : StringPtr(), argv[i_arg]);
            option_index++;
            p_token = nullptr;
        }
    }

    // 解析結果を入れる
    mNumOption = option_index;
    mpOptions = make_unique<std::pair<StringPtr, StringPtr>[]>(nullptr, option_index);
    for (uint32_t i_option = 0; i_option < mNumOption; ++i_option)
    {
        mpOptions[i_option] = *arg_list[i_option];
    }
}

// ----------------------------------------------------------------------------

void ApiEntry::destroy_()
{
    // ルートヒープよりも先に開放すべきものがあればここに記述する
    mpOptions.reset();
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
        exit_code = api_entry.boot(bal::gpApplication, true, __argc, __argv);
    }
    return exit_code;
}

int main(int argc, char* argv[])
{
    // ブート
    int exit_code = 0;
    {
        bal::ApiEntry api_entry;
        exit_code = api_entry.boot(bal::gpApplication, false, argc, argv);
    }
    return exit_code;
}
