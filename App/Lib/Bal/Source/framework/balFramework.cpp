/*!
 * @file   balFramework.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// std
#include <functional>
#include <mutex>
#include <thread>
// Windows
#include <tchar.h>
// bal
#include <app/balApiEntry.h>
#include <app/balApplicationBase.h>
#include <debug/process/balDebugProcessTime.h>
#include <debug/process/balDebugProcessTimeHolder.h>
#include <engine/module/debug/balDebugModule.h>
#include <framework/balFramework.h>
#include <framework/balFrameworkCallback.h>
#include <memory/balSingletonFinalizer.h>
#include <memory/balSingletonGfxFinalizer.h>
#include <time/balStopwatch.h>

// ----------------------------------------------------------------------------

namespace
{

LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY || msg == WM_CLOSE) { PostQuitMessage(0); return 0; }
    return DefWindowProc(handle, msg, wparam, lparam);
}

}

// ----------------------------------------------------------------------------

namespace bal
{

// ----------------------------------------------------------------------------

Framework::Framework()
{

}

// ----------------------------------------------------------------------------

Framework::~Framework()
{
    SingletonFinalizer::Finalize();
}

// ----------------------------------------------------------------------------

void Framework::initialize(const ApiEntry& api_entry, const InitializeArg& arg)
{
    // 保持
    mpApplication = arg.mpApplication;
    if (!mpApplication) { return; }

    // ヒープ取得
    mpRootHeap = api_entry.getRootHeap();

    // アプリケーションモードであれば、アプリケーションウィンドウを用意する
    if (api_entry.getBootMode() == ApiEntry::BootMode::Application)
    {
        // ウィンドウ名を取得
        size_t app_name_len = arg.mTitle.length();
        std::unique_ptr<TCHAR[]> app_name = make_unique<TCHAR[]>(mpRootHeap, app_name_len + 1);
        {
            size_t ret;
#           ifdef UNICODE
            mbstowcs_s(&ret, app_name.get(), app_name_len + 1, arg.mTitle.c_str(), app_name_len + 1);
#           else
            strcpy_s(app_name.get(), app_name_len + 1, arg.mTitle.c_str());
#           endif // UNICODE
        }

        bal::StringPtr current_dir;
        for (uint32_t i_option = 0; i_option < api_entry.getNumOption(); ++i_option)
        {
            if (api_entry.getOptions()[i_option].first.isEquals("-content"))
            {
                current_dir = api_entry.getOptions()[i_option].second;
            }
        }

        // カレントディレクトリの変更
        if (current_dir.isEmpty())
        {
            // 未指定だったら、ROM ディレクトリの ../../Contents にしておく
            std::unique_ptr<TCHAR[]> dir_path = make_unique<TCHAR[]>(mpRootHeap, MAX_PATH);

            // 実行パス取得
            GetModuleFileName(nullptr, dir_path.get(), MAX_PATH);

            // ../../
            TCHAR* p_dir = _tcsrchr(dir_path.get(), _T('\\'));
            if (p_dir) { *p_dir = _T('\0'); }
            p_dir = _tcsrchr(dir_path.get(), _T('\\'));
            if (p_dir) { *p_dir = _T('\0'); }
            p_dir = _tcsrchr(dir_path.get(), _T('\\'));
            if (p_dir) { p_dir = _wcsinc(p_dir); *p_dir = _T('\0'); }

            // Contents
            std::memcpy(p_dir, _T("Contents\\"), sizeof(TCHAR) * 9);
            *(p_dir + 9) = _T('\0');
            SetCurrentDirectory(dir_path.get());
        }
        else
        {
            std::unique_ptr<TCHAR[]> dir_path = make_unique<TCHAR[]>(mpRootHeap, current_dir.size() + 1);
#           if defined(UNICODE)
            size_t ret;
            mbstowcs_s(&ret, dir_path.get(), current_dir.size() + 1, current_dir.c_str(), current_dir.size() + 1);
#           else
            std::memcpy(dir_path.get(), current_dir.c_str(), current_dir.size() + 1);
#           endif
            SetCurrentDirectory(dir_path.get());
        }

        // ウィンドウの登録
        HINSTANCE handle = GetModuleHandle(nullptr);

        WNDCLASSEX wcex;
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.hInstance     = handle;
        wcex.hIcon         = nullptr;
        wcex.hCursor       = nullptr;
        wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wcex.lpszMenuName  = nullptr;
        wcex.lpszClassName = _T("bal");
        wcex.hIconSm       = nullptr;
        if (!RegisterClassEx(&wcex)) { return; }

        DWORD wnd_style    = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
        DWORD wnd_style_ex = 0;

        // ウィンドウサイズを計算
        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = arg.mRenderSize.getWidth();
        rect.bottom = arg.mRenderSize.getHeight();
        AdjustWindowRectEx(&rect, wnd_style, FALSE, wnd_style_ex);

        // ウィンドウを生成
        mHwnd = CreateWindowEx(
            wnd_style_ex,
            _T("bal"), app_name.get(),
            wnd_style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            (rect.right  - rect.left),
            (rect.bottom - rect.top),
            nullptr, nullptr, handle, nullptr
        );
        if (!mHwnd) { return; }

        // グラフィックスの初期化
        mpGraphics = make_unique<Graphics>(nullptr);
        Graphics::InitializeArg init_arg;
        init_arg.mHwnd             = mHwnd;
        init_arg.mRenderBufferSize = arg.mRenderSize;
        init_arg.mBufferCount      = arg.mRenderBufferCount;
        if (!mpGraphics->initialize(init_arg)) { return; }

        // ウィンドウの表示
        ShowWindow(mHwnd, SW_SHOWNORMAL);
        UpdateWindow(mHwnd);

        // アプリケーションモードの時はループが有効になる
        mEnableLoop = true;
    }

    mInitialized = true;
}

// ----------------------------------------------------------------------------

int Framework::enterApplicationLoop(FrameworkCallback* p_callback)
{
    if (!mInitialized)
    {
        // グラフィックスの破棄には順序があるので関数で破棄させる
        SingletonGfxFinalizer::Finalize();
        if (mpGraphics) { mpGraphics->destroy(); }
        return -1;
    }

    // コンソールモードの時はループは不要
    if (!mEnableLoop)
    {
        // グラフィックスの破棄には順序があるので関数で破棄させる
        SingletonGfxFinalizer::Finalize();
        if (mpGraphics) { mpGraphics->destroy(); }
        return -1;
    }

    // コールバックがなければ即終了
    if (!p_callback)
    {
        mEnableLoop = false;
        PostQuitMessage(-1);
    }

    // 保持
    mpCallback = p_callback;

    // ループ用スレッド
    std::thread loop_thread(std::bind(&Framework::applicationLoop_, this));

    // ウィンドウメッセージ確認
    MSG msg;
    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    } while (msg.message != WM_QUIT);

    // 終了処理
    ShowWindow(mHwnd, SW_HIDE);
    mEnableLoop = false;

    // スレッド終了待ち
    loop_thread.join();

    // コマンドの実行待ち
    mpGraphics->waitGPU();

    // グラフィックスの破棄には順序があるので関数で破棄させる
    SingletonGfxFinalizer::Finalize();
    mpGraphics->destroy();

    return static_cast<int>(msg.wParam);
}

// ----------------------------------------------------------------------------

void Framework::applicationLoop_()
{
    // 計測追加
    debug::ProcessHandle process_handle;
    if (mod::debug::Module* p_module = mod::debug::Module::GetModule())
    {
        process_handle = p_module->getProcessTimeHolder()->addThread("MainThread");
    }

    int frame_count = 0;
    Stopwatch sw_for_frame, sw_for_second;
    sw_for_frame.start();
    sw_for_second.start();
    TimeSpan diff(std::chrono::microseconds(16666));

    // コマンド処理
    {
        mpGraphics->postDraw();
        mpGraphics->present(false);
    }

    // ループ処理開始
    while (mEnableLoop)
    {
        // 計算コールバック（nullptr ではない前提で OK）
        {
            FrameworkCallback::UpdateArg update_arg;
            update_arg.mDiff = diff;
            mpCallback->onUpdate(update_arg);
        }

        // 描画コールバック
        {
            {
                //debug::ProcessTime debug_process_time("Wait", MathColor(0.2f, 0.2f, 0.2f));
                // コマンドの実行待ち
                mpGraphics->preDraw();
            }
            {
                debug::ProcessTime debug_process_time("Draw", MathColor(1.f, 0.f, 0.f));
                // コールバック
                FrameworkCallback::DrawArg draw_arg;
                draw_arg.mpSwapChainFrameBuffer = mpGraphics->getSwapChainFrameBuffer();
                draw_arg.mpCommandList          = mpGraphics->getCommandList();
                mpCallback->onDraw(draw_arg);

                // グラフィックスの後処理
                mpGraphics->postDraw();

                // 画面反映
                mpGraphics->present(true);
            }
        }

        // フレームレート計算
        {
            // 毎フレーム
            {
                sw_for_frame.stop();
                diff = sw_for_frame.getDiff();
                sw_for_frame.start();

                mFrameRate = 1.f / (static_cast<float>(diff.getMicroseconds()) * 0.000001f);
            }
            // 毎秒
            {
                frame_count++;
                sw_for_second.lap();
                TimeSpan diff = sw_for_second.getDiff();
                if (diff.getSeconds() >= 1)
                {
                    sw_for_second.start();

                    mFrameRatePerMinute = static_cast<float>(frame_count) / (static_cast<float>(diff.getMicroseconds()) * 0.000001f);
                    frame_count         = 0;
                    BAL_LOG_INFO("FrameRate: %.2f\n", mFrameRatePerMinute);
                }
            }
        }
    }

    // 計測終了
    if (mod::debug::Module* p_module = mod::debug::Module::GetModule())
    {
        p_module->getProcessTimeHolder()->end(process_handle, bal::TimeSpan());
    }
}

// ----------------------------------------------------------------------------

}
