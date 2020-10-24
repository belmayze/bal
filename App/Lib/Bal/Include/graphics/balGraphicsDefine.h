/*!
 * @file   balGraphicsDefine.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

#if BAL_GRAPHICS_IS_D3D12

#   include <graphics/d3d12/balCommandListBundleD3D12.h>
#   include <graphics/d3d12/balCommandListDirectD3D12.h>
#   include <graphics/d3d12/balCommandQueueD3D12.h>
#   include <graphics/d3d12/balConstantBufferD3D12.h>
#   include <graphics/d3d12/balDescriptorHeapD3D12.h>
#   include <graphics/d3d12/balGraphicsD3D12.h>
#   include <graphics/d3d12/balInputLayoutD3D12.h>
#   include <graphics/d3d12/balMeshBufferD3D12.h>
#   include <graphics/d3d12/balPipelineD3D12.h>
#   include <graphics/d3d12/balRenderTargetD3D12.h>
#   include <graphics/d3d12/balTextureD3D12.h>

namespace bal {

using CommandListBundle = d3d12::CommandListBundle;
using CommandListDirect = d3d12::CommandListDirect;
using CommandQueue = d3d12::CommandQueue;
using ConstantBuffer = d3d12::ConstantBuffer;
using DescriptorHeap = d3d12::DescriptorHeap;
using Graphics = d3d12::Graphics;
using InputLayout = d3d12::InputLayout;
using MeshBuffer = d3d12::MeshBuffer;
using Pipeline = d3d12::Pipeline;
using RenderTargetColor = d3d12::RenderTargetColor;
using RenderTargetDepth = d3d12::RenderTargetDepth;
using Texture = d3d12::Texture;

}

#endif // BAL_GRAPHICS_IS_D3D12
