/*!
 * @file   balResourceUpdaterD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balCommandListCopyD3D12.h>
#include <graphics/d3d12/balResourceUpdaterD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------
void ResourceUpdater::initialize(const InitializeArg& arg)
{
    // コンテキスト作成
    mpContexts = make_unique<Context[]>(nullptr, arg.mNumCmdList);
    for (uint32_t i_context = 0; i_context < arg.mNumCmdList; ++i_context)
    {
        mpContexts[i_context].mpCommandList = make_unique<CommandListCopy>(nullptr);
        CommandListCopy::InitializeArg init_arg;
        init_arg.mpGraphics = arg.mpGraphics;
        mpContexts[i_context].mpCommandList->initialize(init_arg);
    }

    // 空きリスト作成
    mpFreeList.initialize(arg.mNumCmdList);
    for (uint32_t i_context = 0; i_context < arg.mNumCmdList; ++i_context)
    {
        mpFreeList.pushBack(&mpContexts[i_context]);
    }
}
// ----------------------------------------------------------------------------
void ResourceUpdater::updateResource(const UpdateArg& arg)
{
    // 空き検索
    Context* p_context = nullptr;
    while (!p_context)
    {
        // 検索する
        {
            std::lock_guard lock(mMutex);
            if (!mpFreeList.empty())
            {
                p_context = *mpFreeList.begin();
                mpFreeList.popFront();
            }
        }
        // 見つからなければ少し待機
        if (!p_context)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    // 実行
    {
        // 初期化
        p_context->mpCommandList->reset();

        // デバイス
        Graphics* p_graphics = reinterpret_cast<Graphics*>(arg.mpGraphics);
        ID3D12Device6* p_device = p_graphics->getDevice();

        // データアップロードに必要なサイズを取得します
        D3D12_RESOURCE_DESC                dst_resource_desc;
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
        UINT64                             row_sizes_in_bytes = 0;
        UINT                               row_counts         = 0;
        size_t                             resource_size      = 0;
        {
            ID3D12Resource* p_resource = static_cast<ID3D12Resource*>(arg.mpDstResource);
            dst_resource_desc = p_resource->GetDesc();
            p_device->GetCopyableFootprints(
                &dst_resource_desc,
                0, 1, 0,
                &layout,
                &row_counts,
                &row_sizes_in_bytes,
                &resource_size
            );
        }

        // アップロード用リソースを作成します
        Microsoft::WRL::ComPtr<ID3D12Resource> p_intermediate;
        {
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment          = 0;
            desc.Width              = resource_size;
            desc.Height             = 1;
            desc.DepthOrArraySize   = 1;
            desc.MipLevels          = 1;
            desc.Format             = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count   = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags              = D3D12_RESOURCE_FLAG_NONE;

            D3D12_HEAP_PROPERTIES props = {};
            props.Type                 = D3D12_HEAP_TYPE_UPLOAD;
            props.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            props.CreationNodeMask     = 1;
            props.VisibleNodeMask      = 1;

            // アップロード用リソース作成
            if (FAILED(p_device->CreateCommittedResource(
                &props,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&p_intermediate)
            )))
            {
                // フリーリストへ戻す
                std::lock_guard lock(mMutex);
                mpFreeList.pushBack(p_context);
                return;
            }

            // アップロード用リソースにデータを転送
            {
                uint8_t* p_gpu_ptr = nullptr;
                p_intermediate->Map(0, nullptr, reinterpret_cast<void**>(&p_gpu_ptr));

                p_gpu_ptr = p_gpu_ptr + layout.Offset;

                for (uint32_t i_row = 0; i_row < row_counts; ++i_row)
                {
                    std::memcpy(
                        p_gpu_ptr + layout.Footprint.RowPitch * i_row,
                        static_cast<const uint8_t*>(arg.mpSrcResource) + layout.Footprint.RowPitch * i_row,
                        row_sizes_in_bytes
                    );
                }

                p_intermediate->Unmap(0, nullptr);
            }
        }

        // コマンド作成
        {
            // バリア
            D3D12_RESOURCE_BARRIER desc = {};
            desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            desc.Transition.pResource   = static_cast<ID3D12Resource*>(arg.mpDstResource);
            desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            desc.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
            desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_DEST;
            //p_context->mpCommandList->getCommandList()->ResourceBarrier(1, &desc);

            // アップロード
            if (dst_resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
            {
                D3D12_BOX box = {};
                box.left   = static_cast<UINT>(layout.Offset);
                box.right  = static_cast<UINT>(layout.Offset + layout.Footprint.Width);
                box.bottom = 1;
                box.back   = 1;
                p_context->mpCommandList->getCommandList()->CopyBufferRegion(
                    static_cast<ID3D12Resource*>(arg.mpDstResource),
                    0,
                    p_intermediate.Get(),
                    layout.Offset,
                    layout.Footprint.Width
                );
            }
            else
            {
                D3D12_TEXTURE_COPY_LOCATION dst;
                D3D12_TEXTURE_COPY_LOCATION src;

                dst.pResource        = static_cast<ID3D12Resource*>(arg.mpDstResource);
                dst.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                dst.SubresourceIndex = 0;

                src.pResource        = p_intermediate.Get();
                src.Type             = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                src.PlacedFootprint  = layout;

                p_context->mpCommandList->getCommandList()->CopyTextureRegion(
                    &dst, 0, 0, 0,
                    &src, nullptr
                );
            }

            // バリア
            desc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_GENERIC_READ;
            //p_context->mpCommandList->getCommandList()->ResourceBarrier(1, &desc);
        }

        // 実行
        p_context->mpCommandList->close();
        p_graphics->executeCopyCommand(*p_context->mpCommandList);
    }

    // フリーリストへ戻す
    {
        std::lock_guard lock(mMutex);
        mpFreeList.pushBack(p_context);
    }
}
// ----------------------------------------------------------------------------
ResourceUpdater::Context::~Context() {}
// ----------------------------------------------------------------------------

}
