/*!
 * @file   balFrameBuffer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/balFrameBuffer.h>
#include <graphics/balICommandList.h>
#include <graphics/balIRenderTarget.h>

namespace bal::gfx {

// ----------------------------------------------------------------------------

FrameBuffer::FrameBuffer()
{
    mpRenderTargetColors.fill(nullptr);
}

// ----------------------------------------------------------------------------

void FrameBuffer::setRenderTargetColor(int index, IRenderTargetColor* p_render_target)
{
    mpRenderTargetColors[index] = p_render_target;
}

// ----------------------------------------------------------------------------

void FrameBuffer::setRenderTargetDepth(IRenderTargetDepth* p_render_target)
{
    mpRenderTargetDepth = p_render_target;
}

// ----------------------------------------------------------------------------

void FrameBuffer::setResolution(const MathSize& size)
{
    mResolution = size;
}

// ----------------------------------------------------------------------------

}
