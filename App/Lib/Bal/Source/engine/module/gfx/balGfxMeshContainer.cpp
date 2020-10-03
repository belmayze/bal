/*!
 * @file   balGfxMeshContainer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxMeshContainer.h>
#include <graphics/balIGraphics.h>
// bal::d3d12
#include <graphics/d3d12/balMeshBufferD3D12.h>

namespace {

// 矩形
const bal::mod::gfx::MeshContainer::Vertex cQuadVertices[] =
{
    {{-1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
    {{-1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
    {{ 1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
    {{ 1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
};
const uint16_t cQuadIndices[] = {0, 1, 2, 0, 2, 3};

}

namespace bal::mod::gfx {

// ----------------------------------------------------------------------------

MeshContainer::MeshContainer() {}

// ----------------------------------------------------------------------------

MeshContainer::~MeshContainer() {}

// ----------------------------------------------------------------------------

void MeshContainer::initialize()
{
    // グラフィックスシステム
    IGraphics* p_graphics = Engine::GetInstance().getGraphicsSystem();

    // 矩形を初期化していくリスト
    using InitializeTuple = std::tuple<std::unique_ptr<IMeshBuffer>*, const Vertex*, size_t, const uint16_t*, size_t>;
    std::array initialize_list =
    {
        InitializeTuple(&mpQuadBuffer, cQuadVertices, sizeof(cQuadVertices), cQuadIndices, sizeof(cQuadIndices))
    };

    // 初期化
    for (InitializeTuple& list : initialize_list)
    {
        *std::get<0>(list) = make_unique<d3d12::MeshBuffer>(nullptr);

        IMeshBuffer::InitializeArg init_arg;
        init_arg.mpGraphics         = p_graphics;
        init_arg.mpVertexBuffer     = std::get<1>(list);
        init_arg.mVertexBufferSize  = std::get<2>(list);
        init_arg.mVertexStride      = sizeof(Vertex);
        init_arg.mpIndexBuffer      = std::get<3>(list);
        init_arg.mIndexBufferSize   = std::get<4>(list);
        init_arg.mIndexBufferFormat = IMeshBuffer::IndexBufferFormat::Uint16;
        init_arg.mIndexCount        = static_cast<uint32_t>(init_arg.mIndexBufferSize / sizeof(uint16_t));
        if (!(*std::get<0>(list))->initialize(init_arg)) { return; }
    }
}

// ----------------------------------------------------------------------------

}
