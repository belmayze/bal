/*!
 * @file   balCommandListD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balCommandListBundleD3D12.h>
#include <graphics/d3d12/balDescriptorHeapD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balMeshBufferD3D12.h>
#include <graphics/d3d12/balPipelineD3D12.h>

namespace bal::d3d12 {

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

void CommandListBundle::bindPipeline(const IPipeline& pipeline)
{
    const Pipeline* p_pipeline = static_cast<const Pipeline*>(&pipeline);
    mpCmdList->SetGraphicsRootSignature(p_pipeline->getRootSignature());
    mpCmdList->SetPipelineState(p_pipeline->getPipelineState());
}

// ----------------------------------------------------------------------------

void CommandListBundle::setDescriptorHeap(uint32_t index, const IDescriptorHeap& descriptor_heap)
{
    const DescriptorHeap* p_descriptor_heap = static_cast<const DescriptorHeap*>(&descriptor_heap);
    ID3D12DescriptorHeap* p_heap = p_descriptor_heap->getDesciptorHeap();
    mpCmdList->SetDescriptorHeaps(1, &p_heap);
    mpCmdList->SetGraphicsRootDescriptorTable(index, p_descriptor_heap->getGpuHandle());
}

// ----------------------------------------------------------------------------

void CommandListBundle::drawMesh(const IMeshBuffer& mesh_buffer)
{
    const MeshBuffer* p_mesh_buffer = static_cast<const MeshBuffer*>(&mesh_buffer);
    D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    switch (p_mesh_buffer->getPrimitiveTopology())
    {
        case IMeshBuffer::PrimitiveTopology::Triangles:
            topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case IMeshBuffer::PrimitiveTopology::Lines:
            topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
    }

    mpCmdList->IASetPrimitiveTopology(topology);
    mpCmdList->IASetVertexBuffers(0, 1, p_mesh_buffer->getVertexBufferView());
    mpCmdList->IASetIndexBuffer(p_mesh_buffer->getIndexBufferView());
    mpCmdList->DrawIndexedInstanced(p_mesh_buffer->getIndexCount(), 1, 0, 0, 0);
}

// ----------------------------------------------------------------------------

}
