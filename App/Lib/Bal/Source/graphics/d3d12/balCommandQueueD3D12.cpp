/*!
 * @file   balCommandQueueD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balArray.h>
#include <graphics/d3d12/balCommandListDirectD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace bal::d3d12 {

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

    // フェンス
    std::unique_ptr<std::unique_ptr<ID3D12Fence, ComDeleter>[]> p_fences = make_unique<std::unique_ptr<ID3D12Fence, ComDeleter>[]>(nullptr, arg.mBufferCount);
    for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
    {
        Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
        {
            if (FAILED(p_device->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
            {
                return false;
            }
        }
        p_fences[i_buffer].reset(p_fence.Detach());
    }

    // 保持
    mpCmdQueue.reset(p_cmd_queue.Detach());
    mpFences = std::move(p_fences);
    mFenceNum = arg.mBufferCount;

    return true;
}

// ----------------------------------------------------------------------------

void CommandQueue::execute(const ICommandListDirect& cmd_list, uint32_t buffer_index)
{
    Array cmd_lists = {reinterpret_cast<const CommandListDirect*>(&cmd_list)->getCommandList()};
    mpCmdQueue->ExecuteCommandLists(1, cmd_lists.data());

    // コマンドの実行が終わったらフェンスが 1 になるようにしておく
    mpFences[buffer_index]->Signal(0);
    mpCmdQueue->Signal(mpFences[buffer_index].get(), 1);
}

// ----------------------------------------------------------------------------

void CommandQueue::waitExecuted(uint32_t buffer_index)
{
    // フェンスが 1 になってない場合は待機する
    if (mpFences[buffer_index]->GetCompletedValue() < 1)
    {
        HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        mpFences[buffer_index]->SetEventOnCompletion(1, handle);
        WaitForSingleObject(handle, INFINITE);
        CloseHandle(handle);
    }
}

// ----------------------------------------------------------------------------

void CommandQueue::waitExecutedAll()
{
    // すべてのフェンスが終了しているかチェック
    for (uint32_t i_fence = 0; i_fence < mFenceNum; ++i_fence)
    {
        waitExecuted(i_fence);
    }

    // キューがすべて無くなったか、最終チェックのフェンスを入れて終わり
    mpFences[0]->Signal(0);
    mpCmdQueue->Signal(mpFences[0].get(), 1);

    HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    mpFences[0]->SetEventOnCompletion(1, handle);
    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);
}

// ----------------------------------------------------------------------------

}
