/*!
 * @file   balGraphicsD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#include <thread>
// bal
#include <io/balFile.h>
#include <graphics/balFrameBuffer.h>
#include <graphics/d3d12/balCommandListDirectD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balDescriptorHeapManagerD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balResourceUpdaterD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------
Graphics::Graphics() {}
// ----------------------------------------------------------------------------
Graphics::~Graphics() {}
// ----------------------------------------------------------------------------
bool Graphics::initialize(const InitializeArg& arg)
{
    // フラグ
    UINT dxgi_factory_flags = 0;

    // デバッグレイヤー
#   if BAL_BUILD_UNDER_DEVELOPMENT
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> p_debug_conrtoller;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&p_debug_conrtoller))))
        {
            p_debug_conrtoller->EnableDebugLayer();
            dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#   endif // BAL_BUILD_UNDER_DEVELOPMENT

    // DXGI
    Microsoft::WRL::ComPtr<IDXGIFactory7> p_factory;
    if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&p_factory))))
    {
        return false;
    }

    // アダプターの検索しながらデバイス取得
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_1_0_CORE;
    std::array initialize_feature_levels =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
    Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
    {
        bool found_adapter = false;
        for (uint32_t i_adapter = 0; p_factory->EnumAdapters1(i_adapter, p_adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i_adapter)
        {
            DXGI_ADAPTER_DESC1 desc;
            p_adapter->GetDesc1(&desc);

            if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
            {
                // ソフトウェア駆動は除外
                p_adapter.Reset();
                continue;
            }
            else
            {
                // ハードウェアなら初期化
                for (D3D_FEATURE_LEVEL level : initialize_feature_levels)
                {
                    if (SUCCEEDED(D3D12CreateDevice(p_adapter.Get(), level, IID_PPV_ARGS(&p_device))))
                    {
                        feature_level = level;
                        found_adapter = true;
                        break;
                    }
                }
            }

            if (found_adapter) { break; }
        }
        if (!found_adapter)
        {
            return false;
        }
    }
    mpDevice.reset(p_device.Detach());

    // コマンドキュー
    mpCmdQueue = make_unique<CommandQueue>(nullptr);
    {
        CommandQueue::InitializeArg init_arg;
        init_arg.mpGraphics   = this;
        init_arg.mType        = CommandQueue::Type::Graphics;
        init_arg.mBufferCount = arg.mBufferCount;
        if (!mpCmdQueue->initialize(init_arg))
        {
            return false;
        }
    }
    // コピーコマンドキュー
    mpCopyCmdQueue = make_unique<CommandQueue>(nullptr);
    {
        CommandQueue::InitializeArg init_arg;
        init_arg.mpGraphics   = this;
        init_arg.mType        = CommandQueue::Type::Copy;
        init_arg.mBufferCount = 1;
        if (!mpCopyCmdQueue->initialize(init_arg))
        {
            return false;
        }
        mpCopyCmdQueue->getFence(0)->Signal(0);
    }

    // コマンドリスト
    mpCmdLists = make_unique<CommandListDirect[]>(nullptr, arg.mBufferCount);
    for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
    {
        CommandListDirect::InitializeArg init_arg;
        init_arg.mpGraphics = this;
        if (!mpCmdLists[i_buffer].initialize(init_arg))
        {
            return false;
        }
    }

    // スワップチェーン
    Microsoft::WRL::ComPtr<IDXGISwapChain3> p_swap_chain;
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width              = arg.mRenderBufferSize.getWidth();
        desc.Height             = arg.mRenderBufferSize.getHeight();
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = arg.mBufferCount;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags              = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
        fullscreen_desc.RefreshRate.Numerator   = 60;
        fullscreen_desc.RefreshRate.Denominator = 1;
        fullscreen_desc.Windowed                = TRUE;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> p_tmp_swap_chain;
        if (FAILED(p_factory->CreateSwapChainForHwnd(
            reinterpret_cast<CommandQueue*>(mpCmdQueue.get())->getCommandQueue(),
            arg.mHwnd,
            &desc, &fullscreen_desc,
            nullptr,
            &p_tmp_swap_chain)))
        {
            return false;
        }

        if (FAILED(p_tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&p_swap_chain))))
        {
            return false;
        }

        // sRGB
        p_swap_chain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709);
    }

    // スワップバッファのテクスチャーをレンダーターゲット化
    mpSwapChainRenderTargets = make_unique<RenderTargetColor[]>(nullptr, arg.mBufferCount);
    mpSwapChainFrameBuffers  = make_unique<FrameBuffer[]>(nullptr, arg.mBufferCount);
    {
        for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
        {
            Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
            if (FAILED(p_swap_chain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_resource))))
            {
                return false;
            }

            std::unique_ptr<Texture> p_color_buffer = make_unique<Texture>(nullptr);
            {
                Texture::InitializeArg init_arg;
                init_arg.mpGraphics  = this;
                init_arg.mpGpuBuffer = p_resource.Detach();
                init_arg.mFormat     = Texture::Format::R8_G8_B8_A8_UNORM;
                p_color_buffer->initialize(init_arg);
            }
            {
                RenderTargetColor::InitializeArg init_arg;
                init_arg.mpGraphics = this;
                mpSwapChainRenderTargets[i_buffer].initialize(init_arg, std::move(p_color_buffer));
            }
            {
                mpSwapChainFrameBuffers[i_buffer].setRenderTargetColor(0, &mpSwapChainRenderTargets[i_buffer]);
                mpSwapChainFrameBuffers[i_buffer].setResolution(arg.mRenderBufferSize);
            }
        }
    }

    // デスクリプターヒープ管理
    {
        DescriptorHeapManager& manager = DescriptorHeapManager::GetInstance();
        DescriptorHeapManager::AddGfxFinalizer();
        DescriptorHeapManager::InitializeArg init_arg;
        init_arg.mpGraphics = this;
        if (!manager.initialize(init_arg))
        {
            return false;
        }
    }

    // リソースアップデーター
    mpResourceUpdater = make_unique<ResourceUpdater>(nullptr);
    {
        ResourceUpdater::InitializeArg init_arg;
        init_arg.mpGraphics  = this;
        init_arg.mNumCmdList = 32;
        mpResourceUpdater->initialize(init_arg);
    }

    // 情報
    mBufferCount = arg.mBufferCount;

    // 必要な COM を保存
    mpSwapChain.reset(p_swap_chain.Detach());

    // 初期化時にもコマンドが使えるように何か書いておく
    preDraw();

    return true;
}
// ----------------------------------------------------------------------------
void Graphics::preDraw()
{
    // バッファー切り替え
    mpCmdQueue->waitExecuted(mCurrentBufferIndex);
    mpCmdLists[mCurrentBufferIndex].reset();
}
// ----------------------------------------------------------------------------
void Graphics::postDraw()
{
    // コマンドリストを閉じて実行
    mpCmdLists[mCurrentBufferIndex].close();
    mpCmdQueue->execute(mpCmdLists[mCurrentBufferIndex], mCurrentBufferIndex);

    // 実行待ち
    if (++mCurrentBufferIndex > (mBufferCount - 1)) { mCurrentBufferIndex = 0; }
}
// ----------------------------------------------------------------------------
void Graphics::present(bool is_sync)
{
    // 画面の反映
    mpSwapChain->Present(is_sync ? 1 : 0, 0);
}
// ----------------------------------------------------------------------------
void Graphics::waitGPU()
{
    mpCmdQueue->waitExecutedAll();
}
// ----------------------------------------------------------------------------
bool Graphics::destroy()
{
    // バッファ破棄
    mpSwapChainFrameBuffers.reset();
    mpSwapChainRenderTargets.reset();

    // アップデータ
    mpResourceUpdater.reset();

    // 破棄
    mpCmdLists.reset();
    mpCmdQueue.reset();
    mpCopyCmdQueue.reset();
    mpSwapChain.reset();
    mpDevice.reset();

    return true;
}
// ----------------------------------------------------------------------------
void Graphics::executeCopyCommand(CommandListCopy& cmd_list)
{
    // @TODO: 複数スレッドの対応が未完成なので実装する
    // ExecuteCommandLists～Signal が順序良く積まれるとは限らない
    uint64_t counter = mCopyCommandCounter.fetch_add(1);

    // インターフェース取得
    ID3D12CommandQueue* p_cmd_queue = mpCopyCmdQueue->getCommandQueue();
    ID3D12Fence*        p_fence     = mpCopyCmdQueue->getFence(0);

    // コマンド実行
    Array<ID3D12CommandList*, 1> cmd_lists = { reinterpret_cast<const CommandListDirect*>(&cmd_list)->getCommandList() };
    p_cmd_queue->ExecuteCommandLists(1, cmd_lists.data());

    // 実行待ち
    p_cmd_queue->Signal(p_fence, counter);
    HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    p_fence->SetEventOnCompletion(1, handle);
    if (p_fence->GetCompletedValue() < counter)
    {
        WaitForSingleObject(handle, INFINITE);
    }
    CloseHandle(handle);
}
// ----------------------------------------------------------------------------
ICommandListDirect* Graphics::getCommandList()
{
    return &mpCmdLists[mCurrentBufferIndex];
}
// ----------------------------------------------------------------------------
FrameBuffer* Graphics::getSwapChainFrameBuffer()
{
    return &mpSwapChainFrameBuffers[mCurrentBufferIndex];
}
// ----------------------------------------------------------------------------
ITexture::Format Graphics::getSwapChainColorFormat()
{
    return mpSwapChainRenderTargets[0].getTexture()->getFormat();
}
// ----------------------------------------------------------------------------

}
