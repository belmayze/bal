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

namespace bal {

// ----------------------------------------------------------------------------

bool Engine::initialize(const InitializeArg& arg)
{
    return true;
}

// ----------------------------------------------------------------------------

void Engine::onUpdate(const UpdateArg& arg)
{

}

// ----------------------------------------------------------------------------

void Engine::onDraw(const DrawArg& arg)
{
    // バリア
    arg.mpCommandList->resourceBarrier(
        *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
        bal::gfx::ICommandListDirect::ResourceBarrierType::Present,
        bal::gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor
    );

    // スワップバッファをとりあえず適当にクリア
    arg.mpCommandList->clear(
        *arg.mpSwapChainFrameBuffer,
        bal::gfx::ICommandListDirect::ClearFlag::Color,
        bal::MathColor(1.f, 0.f, 0.f, 1.f),
        1.f, 0
    );

    // バリア
    arg.mpCommandList->resourceBarrier(
        *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
        bal::gfx::ICommandListDirect::ResourceBarrierType::RenderTargetColor,
        bal::gfx::ICommandListDirect::ResourceBarrierType::Present
    );
}

// ----------------------------------------------------------------------------

}
