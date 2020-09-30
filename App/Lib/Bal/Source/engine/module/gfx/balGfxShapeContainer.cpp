/*!
 * @file   balGfxModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxShapeContainer.h>
#include <graphics/balIGraphics.h>
// bal::d3d12
#include <graphics/d3d12/balShapeBufferD3D12.h>

namespace bal::mod::gfx {

// ----------------------------------------------------------------------------

ShapeContainer::ShapeContainer() {}

// ----------------------------------------------------------------------------

ShapeContainer::~ShapeContainer() {}

// ----------------------------------------------------------------------------

void ShapeContainer::initialize()
{
    // グラフィックスシステム
    IGraphics* p_graphics = Engine::GetInstance().getGraphicsSystem();

    // 矩形
    {
        mpQuadBuffer = make_unique<d3d12::ShapeBuffer>(nullptr);

        // position, texcoord
        float vertices[] = {
            -1.f,  1.f, 0.f,  0.f, 0.f,
            -1.f, -1.f, 0.f,  0.f, 1.f,
             1.f, -1.f, 0.f,  1.f, 1.f,
             1.f,  1.f, 0.f,  1.f, 0.f
        };
        uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

        IShapeBuffer::InitializeArg init_arg;
        init_arg.mpGraphics         = p_graphics;
        init_arg.mpVertexBuffer     = reinterpret_cast<const uint8_t*>(vertices);
        init_arg.mVertexBufferSize  = sizeof(vertices);
        init_arg.mVertexStride      = sizeof(float) * 5;
        init_arg.mpIndexBuffer      = reinterpret_cast<const uint8_t*>(indices);
        init_arg.mIndexBufferSize   = sizeof(indices);
        init_arg.mIndexCount        = 6;
        init_arg.mIndexBufferFormat = IShapeBuffer::IndexBufferFormat::Uint16;
        if (!mpQuadBuffer->initialize(init_arg)) { return; }
    }
}

// ----------------------------------------------------------------------------

}
