/*!
 * @file   balEngine.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
// gfx
#include <graphics/balFrameBuffer.h>
#include <graphics/balICommandListDirect.h>
#include <graphics/balIRenderTarget.h>
#include <graphics/balViewport.h>
#include <graphics/archiver/balShaderArchive.h>
// gfx::d3d12
#include <graphics/d3d12/balTextureD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>

namespace bal {

// ----------------------------------------------------------------------------

Engine::Engine() {}

// ----------------------------------------------------------------------------

Engine::~Engine() {}

// ----------------------------------------------------------------------------

bool Engine::initialize(const InitializeArg& arg)
{
    // HDR 用レンダーバッファ
    {
        // テクスチャーを確保
        std::unique_ptr<gfx::ITexture> p_color_buffer = make_unique<gfx::d3d12::Texture>(nullptr);
        {
            gfx::ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = arg.mpGraphics;
            init_arg.mDimension = gfx::ITexture::Dimension::Texture2D;
            init_arg.mFormat    = gfx::ITexture::Format::R16_G16_B16_A16_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!p_color_buffer->initialize(init_arg)) { return false; }
        }
        std::unique_ptr<gfx::ITexture> p_depth_buffer = make_unique<gfx::d3d12::Texture>(nullptr);
        {
            gfx::ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = arg.mpGraphics;
            init_arg.mDimension = gfx::ITexture::Dimension::Texture2D;
            init_arg.mFormat    = gfx::ITexture::Format::D32_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!p_depth_buffer->initialize(init_arg)) { return false; }
        }

        // レンダーターゲット
        mpRenderTargetColor = make_unique<gfx::d3d12::RenderTargetColor>(nullptr);
        {
            gfx::IRenderTargetColor::InitializeArg init_arg;
            init_arg.mpGraphics = arg.mpGraphics;
            if (!mpRenderTargetColor->initialize(init_arg, std::move(p_color_buffer))) { return false; }
        }
        mpRenderTargetDepth = make_unique<gfx::d3d12::RenderTargetDepth>(nullptr);
        {
            gfx::IRenderTargetDepth::InitializeArg init_arg;
            init_arg.mpGraphics = arg.mpGraphics;
            if (!mpRenderTargetDepth->initialize(init_arg, std::move(p_depth_buffer))) { return false; }
        }

        // フレームバッファ
        mpFrameBuffer = make_unique<gfx::FrameBuffer>(nullptr);
        {
            mpFrameBuffer->setRenderTargetColor(0, mpRenderTargetColor.get());
            mpFrameBuffer->setRenderTargetDepth(mpRenderTargetDepth.get());
            mpFrameBuffer->setResolution(arg.mRenderBufferSize);
        }
    }

    // シェーダーアーカイブ読み込み
    mpShaderArchive = make_unique<gfx::ShaderArchive>(nullptr);
    {
        File file;
        if (!file.loadFromFile("Shader\\bal_shader.bsa")) { return false; }
        if (!mpShaderArchive->loadArchiver(std::move(file))) { return false; }
    }

    return true;
}

// ----------------------------------------------------------------------------

void Engine::onUpdate(const UpdateArg& arg)
{

}

// ----------------------------------------------------------------------------

void Engine::onDraw(const DrawArg& arg)
{
    // レンダリング用バッファに切り替え
    {
        // バリア
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetColor->getTexture(),
            gfx::ICommandListDirect::ResourceBarrierType::GenericRead,
            gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor
        );
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetDepth->getTexture(),
            gfx::ICommandListDirect::ResourceBarrierType::GenericRead,
            gfx::ICommandListDirect::ResourceBarrierType::RenderTargetDepth
        );

        // バッファ切り替え
        arg.mpCommandList->setViewport(gfx::Viewport(*mpFrameBuffer));
        arg.mpCommandList->bindFrameBuffer(*mpFrameBuffer);
        arg.mpCommandList->clear(*mpFrameBuffer, gfx::ICommandListDirect::ClearFlag::Color | gfx::ICommandListDirect::ClearFlag::Depth, MathColor(0.f, 0.f, 0.f, 1.f), 1.f, 0);

        // @TODO: レンダリング

        // バリア
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetColor->getTexture(),
            gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor,
            gfx::ICommandListDirect::ResourceBarrierType::GenericRead
        );
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetDepth->getTexture(),
            gfx::ICommandListDirect::ResourceBarrierType::RenderTargetDepth,
            gfx::ICommandListDirect::ResourceBarrierType::GenericRead
        );
    }

    // レンダリング用を書き出し
    {
        // バリア
        arg.mpCommandList->resourceBarrier(
            *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
            bal::gfx::ICommandListDirect::ResourceBarrierType::Present,
            bal::gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor
        );

        // スワップバッファをとりあえず適当にクリア
        //arg.mpCommandList->clear(
        //    *arg.mpSwapChainFrameBuffer,
        //    bal::gfx::ICommandListDirect::ClearFlag::Color,
        //    bal::MathColor(1.f, 0.f, 0.f, 1.f),
        //    1.f, 0
        //);

        // バリア
        arg.mpCommandList->resourceBarrier(
            *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
            bal::gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor,
            bal::gfx::ICommandListDirect::ResourceBarrierType::Present
        );
    }
}

// ----------------------------------------------------------------------------

}
