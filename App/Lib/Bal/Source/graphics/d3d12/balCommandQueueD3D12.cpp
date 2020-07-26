/*!
 * @file   balCommandQueueD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/d3d12/balCommandListD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool CommandQueue::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 種類
    D3D12_COMMAND_LIST_TYPE type;
    switch (arg.mType)
    {
    case Type::Graphics: type = D3D12_COMMAND_LIST_TYPE_DIRECT; break;
    case Type::Compute:  type = D3D12_COMMAND_LIST_TYPE_COMPUTE; break;
    }

    // キュー
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> p_cmd_queue;
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = type;

        if (FAILED(p_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&p_cmd_queue))))
        {
            return false;
        }
    }

    // フェンスとイベントハンドル
    HANDLE event_handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
    {
        if (FAILED(p_device->CreateFence(mSignalValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
        {
            return false;
        }
    }

    // 保持
    mpCmdQueue.reset(p_cmd_queue.Detach());
    mpFence.reset(p_fence.Detach());
    mEventHandle = event_handle;

    return true;
}

// ----------------------------------------------------------------------------

void CommandQueue::execute(const ICommandList* p_cmd_list)
{
    ID3D12CommandList* cmd_lists[] = {reinterpret_cast<const CommandList*>(p_cmd_list)->getCommandList()};
    mpCmdQueue->ExecuteCommandLists(1, cmd_lists);
}

// ----------------------------------------------------------------------------

void CommandQueue::waitExecuted()
{
    mpFence->Signal(mSignalValue);
    mpCmdQueue->Signal(mpFence.get(), mSignalValue + 1);
    mSignalValue++;

    if (mpFence->GetCompletedValue() < mSignalValue)
    {
        mpFence->SetEventOnCompletion(mSignalValue, mEventHandle);
        WaitForSingleObject(mEventHandle, INFINITE);
    }
}

// ----------------------------------------------------------------------------

}
