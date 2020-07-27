/*!
 * @file   balCommandListD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/d3d12/balCommandListBundleD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool CommandListBundle::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 種類
    D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_BUNDLE;

    // アロケーター
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_cmd_allocator;
    if (FAILED(p_device->CreateCommandAllocator(type, IID_PPV_ARGS(&p_cmd_allocator))))
    {
        return false;
    }

    // コマンドリスト
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_cmd_list;
    if (FAILED(p_device->CreateCommandList(1, type, p_cmd_allocator.Get(), nullptr, IID_PPV_ARGS(&p_cmd_list))))
    {
        return false;
    }
    p_cmd_list->Close();

    // 保持
    mpCmdAllocator.reset(p_cmd_allocator.Detach());
    mpCmdList.reset(p_cmd_list.Detach());

    return true;
}

// ----------------------------------------------------------------------------

void CommandListBundle::reset()
{
    mpCmdAllocator->Reset();
    mpCmdList->Reset(mpCmdAllocator.get(), nullptr);
}

// ----------------------------------------------------------------------------

void CommandListBundle::close()
{
    mpCmdList->Close();
}

// ----------------------------------------------------------------------------

}
