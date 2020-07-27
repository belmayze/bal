/*!
 * @file   balShaderD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balShaderD3D12.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool Shader::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // @TODO: いろいろ固定してあるので、変更する

    // パイプラインを生成
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline_state;
    {
        // レイアウト
        D3D12_INPUT_ELEMENT_DESC element_desc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
        };

        // ラスタライザー
        D3D12_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.FillMode              = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode              = D3D12_CULL_MODE_BACK;
        rasterizer_desc.FrontCounterClockwise = false;
        rasterizer_desc.DepthClipEnable       = true;

        // ブレンダー
        D3D12_BLEND_DESC blend_desc = {};
        blend_desc.RenderTarget[0].BlendEnable           = false;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // @TODO: パイプライン（本当にシェーダーでいい？）

    }

    return true;
}

// ----------------------------------------------------------------------------

}
