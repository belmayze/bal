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
#include <framework/balFramework.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <heap/balHeapManager.h>
#include <memory/balSingletonFinalizer.h>
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
    // ルートヒープを確保
    mpRootHeap = heap::RootBlock::Create(arg.mHeapSize);
    heap::Manager::GetInstance(mpRootHeap.get()).setCurrentHeap(mpRootHeap.get());

    // アプリケーション保持
    mpApplication = arg.mpApplication;
    if (!mpApplication) { return; }

    // アプリケーションモードであれば、アプリケーションウィンドウを用意する
    if (api_entry.getBootMode() == ApiEntry::BootMode::Application)
    {
        // ウィンドウ名を取得
        size_t app_name_len = arg.mTitle.length();
        std::unique_ptr<TCHAR[]> app_name = make_unique<TCHAR[]>(mpRootHeap.get(), app_name_len + 1);
        {
            size_t ret;
#           ifdef UNICODE
            mbstowcs_s(&ret, app_name.get(), app_name_len + 1, arg.mTitle.c_str(), app_name_len + 1);
#           else
            strcpy_s(app_name.get(), app_name_len + 1, arg.mTitle.c_str());
#           endif // UNICODE
        }

        // カレントディレクトリの変更
        {
            std::unique_ptr<TCHAR[]> dir_path = make_unique<TCHAR[]>(mpRootHeap.get(), MAX_PATH);
            GetModuleFileName(nullptr, dir_path.get(), MAX_PATH);
            TCHAR* p_dir = _tcsrchr(dir_path.get(), _T('\\'));
            if (p_dir)
            {
                p_dir  = _wcsinc(p_dir);
                *p_dir = _T('\0');
                SetCurrentDirectory(dir_path.get());
            }
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
        mpGraphics = make_unique<gfx::d3d12::Graphics>(nullptr);
        gfx::IGraphics::InitializeArg init_arg;
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

void Framework::enterApplicationLoop()
{
    if (!mInitialized) { return; }

    // コンソールモードの時はループは不要
    if (!mEnableLoop)
    {
        mpApplication->onLoop();
        return;
    }

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

    // @TODO: グラフィックスの終了処理
    mpGraphics->destroy();
    mpGraphics.reset();
}

// ----------------------------------------------------------------------------

void Framework::applicationLoop_()
{
    int frame_count = 0;
    Stopwatch sw_for_frame, sw_for_second;
    sw_for_frame.start();
    sw_for_second.start();

    // ループ処理開始
    while (mEnableLoop)
    {
        if (!mpApplication->onLoop())
        {
            PostQuitMessage(0);
            return;
        }
        // @Debug
        reinterpret_cast<gfx::d3d12::Graphics*>(mpGraphics.get())->loop();

        // フレームレート計算
        {
            // 毎フレーム
            {
                sw_for_frame.stop();
                TimeSpan diff = sw_for_frame.getDiff();
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
                    Log::Print("FrameRate: %.2f\n", mFrameRatePerMinute);
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------

}
