/*!
 * @file   balFrameBufferD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/balFrameBuffer.h>
#include <graphics/balViewport.h>

namespace bal::gfx {

// ----------------------------------------------------------------------------

Viewport::Viewport(const FrameBuffer& frame_buffer)
    : mOrigin(MathVector2(0.f, 0.f))
    , mSize(MathVector2(static_cast<float>(frame_buffer.getResolution().getWidth()), static_cast<float>(frame_buffer.getResolution().getHeight())))
    , mMinDepth(0.f), mMaxDepth(1.f) { }

// ----------------------------------------------------------------------------

}
